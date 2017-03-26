# Wilcoxon test from file

file.name <- "score.txt"

# Read file
all.data <- read.table(file=file.name, header=TRUE, sep=":") #Replace the separator for any of your preference
trials <- all.data[,1]  # Remove this line if there are no trial/instances
data      <- all.data[,-1, drop=FALSE] # Get the data

#compute result and print them into "wilcoxon-algo.txt"
source("wilcoxon.R")
do.wilcoxon(data.matrix=data, output="wilcoxon-algo.txt")