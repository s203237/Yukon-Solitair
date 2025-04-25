#include "util.h"
#include <stdbool.h>
#include "card.h"


int rankToValue(char rank){
    if(rank>='2'&& rank<=9) return rank -'0';
    switch(rank){
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        case'A': return 1;
    }
    return -1;
}

bool isRed(char suit){
    return suit =='H'|| suit=='D';
}
bool isValidMove(Card* fromCard, Card* toCard ){
 if (!fromCard ||!toCard) return false;
 if(isRed(fromCard->suit)==isRed(toCard->suit)){return false;}
 int fromVal = rankToValue(fromCard->rank);
 int toVal = rankToValue(toCard->rank);
 return fromVal == toVal - 1;

}