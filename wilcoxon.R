#data.matrix is a matrix, each test is a column
#  colnames are the names of the tests  
#  output: name of the file to write the results in
do.wilcoxon <- function(data.matrix, type="two.sided", output=NULL){
    
    #options(digits=3)
    
    print(data.matrix)

    # Test result saving structure  
    wilcoxon.results <- matrix(NA, ncol=ncol(data.matrix), nrow=ncol(data.matrix))
    colnames(wilcoxon.results) <- colnames(data.matrix)
    rownames(wilcoxon.results) <- colnames(data.matrix)
        
    test.names <- colnames(data.matrix) #get the test names
    pvals<-c()

    #Perform the test in all the combinations of tests
    for(i in 1:(length(test.names))){
        for(j in 1:length(test.names)){
            test <- wilcox.test(data.matrix[,i], data.matrix[,j], paired=TRUE, alternative=type) # paired = TRUE, add when used the wilcoxon signed rank test otherwise is the wilcoxon rank sum test
            wilcoxon.results[test.names[i],test.names[j]]<- test$p.value
            #test.results[[test.name]][algorithms[j],algorithms[i]]<- test$p.value
            pvals <- c(pvals, test$p.value)
        }
    }

    if(!is.null(output)){
      write("\nWilcoxon signed rank (paired):\n",file=output, append=TRUE)
      write.table(format(wilcoxon.results, digits = 3), file=output, sep=" & ", append=TRUE)
    }

    # #Correct the test (this is done due to the multiple comparisons)
    # pvals <- p.adjust(pvals,method="bonferroni")
    # s <- 1
    # for(i in 1:(length(test.names))){
    #     for(j in 1:length(test.names)){
    #        wilcoxon.results[test.names[i],test.names[j]] <- pvals[s]
    #        s <- s+1
    #     }
    # }    
    # if(!is.null(output)){
    #    write("\n -> Bonferroni correction:\n", file=output, append=TRUE)
    #    write.table( format(wilcoxon.results, digits = 3), file=output, append=TRUE, sep=" & ")
    # }
    # cat("Wilcoxon test + bonferroni correction:\n")
    # return(wilcoxon.results)
    
}

