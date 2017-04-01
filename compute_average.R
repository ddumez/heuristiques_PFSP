file.name <- "tmp.txt"
output = "avg.txt"

# Read file
all.data <- read.table(file=file.name, header=TRUE, sep=":") #Replace the separator for any of your preference
trials <- all.data[,1]  # Remove this line if there are no trial/instances
data      <- all.data[,-1, drop=FALSE] # Get the data

#Compute and print into avg.txt
x = colMeans(data, na.rm = TRUE, dims = 1)
x = 100 *x
write.table(x, file=output, sep="\t", append=TRUE)