#include <iostream>

struct Granny {};
struct Dad: virtual Granny {};
struct Mom: virtual Granny {};
struct Son: Mom, Dad {};


int main(){
    try{
        Son s;
        throw s;
    } catch (Granny){
        std::cout << "caught Granny";
    } catch (Son){
        std::cout << "caught Son";
    } catch (...){
        std::cout << "other";
    }
}