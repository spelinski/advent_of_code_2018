//THIS IS A SAMPLE TO TO GET MAKEFILE WORKING BEFORE ADVENT STARTS
namespace temp {
    auto user_sum(unsigned int limit)
    {
        auto sum = 0ull;
        for(auto i = 1u; i< limit; ++i){
            if(i%3 == 0 || i%5 == 0) { sum += i; }
        }
        return sum;
    }
}