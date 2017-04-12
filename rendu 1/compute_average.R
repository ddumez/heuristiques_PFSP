file.name <- "tmp.txt"
output = "avg.txt"

# Read file
all.data <- read.table(file=file.name, header=TRUE, sep=":") #Replace the separator for any of your preference
trials <- all.data[,1]  # Remove this line if there are no trial/instances
data      <- all.data[,-1, drop=FALSE] # Get the data

#Compute and print into avg.txt
# data = 100*data #when we want to put relative devaition in %
x = colMeans(data, na.rm = TRUE, dims = 1)
write.table(x, file=output, sep="\t", append=TRUE)