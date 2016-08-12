// [[Rcpp::depends(RQuantLib)]]
// [[Rcpp::plugins(cpp11)]]

#include <ql/instruments/bond.hpp>
#include <boost/timer.hpp>
using namespace QuantLib;


#include <boost/timer.hpp>
#include <vector>
using namespace QuantLib;

// [[Rcpp::export]]
double fixedRateBond(Date settlementDate,
                     Date maturity,
                     Real couponRate,
                     Real yield,
                     const DayCounter& accrualDayCounter = ActualActual(ActualActual::Bond),
                     int freqPerYear = 2,
                     const Date& issueDate = Date(),
                     Real faceAmount = 100.0,
                     BusinessDayConvention paymentConvention = Following,
                     bool exCouponEndOfMonth = false){
    
    boost::timer timer;
    Calendar calendar = TARGET();
    settlementDate = calendar.adjust(settlementDate);
    Integer fixingDays = 1;
    Natural settlementDays = 1;
    Date todaysDate = settlementDate.todaysDate();
    //calendar.advance(settlementDate, -fixingDays, Days);
    Settings::instance().evaluationDate() = todaysDate;
    Frequency val = static_cast<Frequency>(freqPerYear);
    
    RelinkableHandle<YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<YieldTermStructure> flatTermStructure(
                                                            new FlatForward(settlementDate,
                                                                            yield,
                                                                            accrualDayCounter ,
                                                                            Compounding::Compounded,
                                                                            val));
    discountingTermStructure.linkTo(flatTermStructure);
    boost::shared_ptr<PricingEngine> bondEngine(
                                                new DiscountingBondEngine(discountingTermStructure));
    
    Schedule fixedBondSchedule(
                               issueDate,
                               maturity,
                               Period(val),
                               UnitedStates(UnitedStates::GovernmentBond),
                               BusinessDayConvention::Unadjusted,
                               BusinessDayConvention::Unadjusted,
                               DateGeneration::Rule::Backward,
                               exCouponEndOfMonth);
    
    FixedRateBond fixedRateBond(
                                settlementDays,
                                faceAmount,
                                fixedBondSchedule,
                                std::vector<Rate>(1, couponRate),
                                accrualDayCounter,
                                BusinessDayConvention::Unadjusted,
                                faceAmount,
                                issueDate);
    fixedRateBond.setPricingEngine(bondEngine);
    
    
    
    RelinkableHandle<YieldTermStructure> discountingTermStructure2;
    boost::shared_ptr<YieldTermStructure> flatTermStructure2(
                                                            new FlatForward(settlementDate,
                                                                            yield,
                                                                            accrualDayCounter ,
                                                                            Compounding::Compounded,
                                                                            val));
    discountingTermStructure2.linkTo(flatTermStructure2);
    boost::shared_ptr<PricingEngine> bondEngine2(
                                                new DiscountingBondEngine(discountingTermStructure2));
    
    boost::shared_ptr<IborIndex> libor(
                                       new USDLibor(Period(1,Years),discountingTermStructure2));
    //libor->addFixing( settlementDate-- , yield);
    
    FloatingRateBond floatingRateBond(settlementDays,
                                      faceAmount,
                                      fixedBondSchedule,
                                      libor,
                                      accrualDayCounter,
                                      BusinessDayConvention::Unadjusted,
                                      fixingDays);
    floatingRateBond.setPricingEngine(bondEngine);
    
    return fixedRateBond.NPV() - floatingRateBond.NPV();
}

