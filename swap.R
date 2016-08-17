swapByFlatRate <- function(issueDate ,
                           maturityDate ,
                           tradeDate , 
                           rates ,
                           yield   ,
                           faceAmount=100, 
                           freq  = 2,
                           settleDate  = tradeDate
                           ){
      
      bond <- list(faceAmount= faceAmount, 
                   issueDate= issueDate,
                   maturityDate=maturityDate
      )
      
      schedule <- list(effectiveDate=tradeDate,
                       maturityDate=maturityDate,
                       period=freq)
      
      
      params <- list(tradeDate=tradeDate,
                     settleDate= settleDate,
                     dt=1
                     )
      setEvaluationDate( tradeDate )
      
      tsQuotes <- list(flat=yield,
                       d1w = 0,
                       d1m = 0,
                       d3m = 0,
                       d6m = 0,
                       d9m = 0,
                       d1y = 0,
                       fut1= 100,
                       fut2=100,
                       fut3=100,
                       fut4=100,
                       fut5=100,
                       fut6=100,
                       fut7=100,
                       fut8=100,
                       #                 s2y = 0.037125,
                       s3y = 0,
                       s5y = 0,
                       s10y = 0,
                       s15y = 0)
      
      discountCurve.flat <- DiscountCurve(params, tsQuotes)
      termstructure <- DiscountCurve(params, tsQuotes)
      iborIndex.params <- list(type="USDLibor", length= 12, 
                               inTermOf="Month", term=termstructure)  
      dateparams <- list(settlementDays=1, calendar="UnitedStates/GovernmentBond",
                          period=2, 
                         businessDayConvention = 1, terminationDateConvention=1,
                         dateGeneration=0, endOfMonth=0, fixingDays = 1)
      
      
      FloatingRateBond(bond, 
                       index = iborIndex.params,
                       curve = discountCurve.flat,
                       dateparams  = dateparams )-
            FixedRateBond(bond, rates, schedule , yield = yield)
      
      
      
}
