library(websocket)
library(promises)
library(future)

prices <- data.frame(matrix(ncol = 3, nrow = 0))
colnames(prices) <- c("time", "best_bid", "best_offer")
continue <- T
window_size <- 100

update_bbbo <- function () {
  msg <- promise(function(resolve, reject) {
    ws <- WebSocket$new("ws://localhost:8765")
    ws$onOpen(function(e) ws$send("bbbo"))
    ws$onMessage(function(e) resolve(e$data))
  })
  
  then(msg,
    onFulfilled = function(v) {
      bb <- as.numeric(strsplit(v, "\\|")[[1]][2])
      bo <- as.numeric(strsplit(v, "\\|")[[1]][3])
      time_ns <- as.numeric(strsplit(v, "\\|")[[1]][4])
      
      prices[nrow(prices) + 1,] <<- list(time_ns, bb, bo)
      if (nrow(prices) > window_size) {
        view_prices <- prices[(nrow(prices)-window_size):nrow(prices),]
      } else {
        view_prices <- prices
      }
      
      plot(best_bid ~ time, data = view_prices, col = "red", type = "l",
           ylim = c(min(min(view_prices$best_bid), min(view_prices$best_offer)) * 0.99, max(max(view_prices$best_bid), max(view_prices$best_offer)) * 1.01),
           xlab = "Time", ylab = "Price")
      lines(best_offer ~ time, data = view_prices, col = "blue", type = "l")
      
      legend("topleft", legend=c("Best bid", "Best offer"),
             col=c("red", "blue"), lty=c(1,1), cex=1)
      
      if (continue) {
        update_bbbo()  
      }
    },
    onRejected = function() {
      print("Failed to get BBBO")
      if (continue) {
        update_bbbo()
      }
    }
  )
}

update_bbbo()

continue <- F