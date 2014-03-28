/*
 * compute_routines.h
 *
 *  Created on: Mar 26, 2014
 *      Author: deepan


   Contains routines to do feature updates
 */
#pragma once
#ifndef COMPUTE_ROUTINES_H_
#define COMPUTE_ROUTINES_H_

#include "class_definitions.h"

oneDState levelOneBookToState(const levelOneBook& lob);
void updateFeatures(oneDState& a_ODS, const levelOneBook& a_currentLevelOneBook, const levelOneBook& a_pastLevelOneBook);
void updateConditionalFeatures(vector<levelOneBook>& a_closingConditionalBooks, vector<oneDState>& a_positiveStates,
                               vector<oneDState>& a_negativeStates, oneDState& a_balancedState, int a_futureIndex);



void updateFeatures(vector<oneDState>& a_positiveStates, vector<oneDState>& a_negativeStates,
		            oneDState& a_balancedState, const levelOneBook& a_currentLevelOneBook, const levelOneBook& a_pastLevelOneBook)
  {
    long double midPriceChange = (static_cast<long double>(a_currentLevelOneBook.bidPrice+a_currentLevelOneBook.askPrice))/2.0
                                 -(static_cast<long double>(a_pastLevelOneBook.bidPrice+a_pastLevelOneBook.askPrice))/2.0;
    //long double askPriceChange = (static_cast<long double>(a_currentLevelOneBook.askPrice))-(static_cast<long double>(a_pastLevelOneBook.askPrice));
    if(a_pastLevelOneBook.bidShares > a_pastLevelOneBook.askShares)
       {
           int a_imb = min(a_pastLevelOneBook.bidShares/100-a_pastLevelOneBook.askShares/100, static_cast<uint32_t>(MAX_IMBALANCE));
    	   a_positiveStates[a_imb].stateFeatures.meanMidPriceChange = (a_positiveStates[a_imb].stateFeatures.meanMidPriceChange
    			                                                     *(static_cast<long double>(a_positiveStates[a_imb].timeSpan))
                   + midPriceChange)/(static_cast<long double>(a_positiveStates[a_imb].timeSpan)+1.0);
    	   a_positiveStates[a_imb].timeSpan++;
       }
   else if(a_pastLevelOneBook.bidShares < a_pastLevelOneBook.askShares)
      {
	       int a_imb = min(a_pastLevelOneBook.askShares/100-a_pastLevelOneBook.bidShares/100, static_cast<uint32_t>(MAX_IMBALANCE));
	       	   a_negativeStates[a_imb].stateFeatures.meanMidPriceChange = (a_negativeStates[a_imb].stateFeatures.meanMidPriceChange
	       			                                                     *(static_cast<long double>(a_negativeStates[a_imb].timeSpan))
	                      + midPriceChange)/(static_cast<long double>(a_negativeStates[a_imb].timeSpan)+1.0);
	       a_negativeStates[a_imb].timeSpan++;
      }
   else
   	{
	       a_balancedState.stateFeatures.meanMidPriceChange = (a_balancedState.stateFeatures.meanMidPriceChange
			                                               *(static_cast<long double>(a_balancedState.timeSpan))
	                                                + midPriceChange)/(static_cast<long double>(a_balancedState.timeSpan)+1.0);
	       a_balancedState.timeSpan++;
    }



    return;
  }

void updateConditionalFeatures(vector<levelOneBook>& a_closingConditionalBooks, vector<oneDState>& a_positiveStates,
                               vector<oneDState>& a_negativeStates, oneDState& a_balancedState, int a_futureIndex)
{
   levelOneBook futureLevelOneBook = a_closingConditionalBooks[a_futureIndex];
   vector<levelOneBook>::iterator it = a_closingConditionalBooks.begin();
   int a_index = 0;
   while(it!=a_closingConditionalBooks.end() && a_index < a_futureIndex)
     {
			if((it->bidShares) > (it->askShares))
   			{
        		   long double midPriceChange = static_cast<long double>(futureLevelOneBook.bidPrice+futureLevelOneBook.askPrice)/2.0-
                                            static_cast<long double>(it->bidPrice+it->askPrice)/2.0;
               long double timeToMidPriceChange = static_cast<long double>(a_futureIndex - a_index);
               oneDState a_ODS = a_positiveStates[min(it->bidShares/100-it->askShares/100,static_cast<uint32_t>(MAX_IMBALANCE))];
               int a_imb = min(it->bidShares/100-it->askShares/100,static_cast<uint32_t>(MAX_IMBALANCE));
               a_positiveStates[a_imb].stateFeatures.conditionalMeanMidPriceChange
               = (a_ODS.stateFeatures.conditionalMeanMidPriceChange*(static_cast<long double>(a_ODS.numberOfMidPriceChanges))+ midPriceChange)
                 /(static_cast<long double>(a_ODS.numberOfMidPriceChanges)+1.0);
               a_positiveStates[a_imb].stateFeatures.timeToMidPriceChange
               = (a_ODS.stateFeatures.timeToMidPriceChange*(static_cast<long double>(a_ODS.numberOfMidPriceChanges))+ midPriceChange)
                 /(static_cast<long double>(a_ODS.numberOfMidPriceChanges)+1.0);
               a_positiveStates[a_imb].numberOfMidPriceChanges++;

      		}
			else if((it->bidShares) < (it->askShares))
   			{
               long double midPriceChange = static_cast<long double>(futureLevelOneBook.bidPrice+futureLevelOneBook.askPrice)/2.0-
                                            static_cast<long double>(it->bidPrice+it->askPrice)/2.0;
               long double timeToMidPriceChange = static_cast<long double>(a_futureIndex - a_index);
               oneDState a_ODS = a_negativeStates[min(it->askShares/100-it->bidShares/100,static_cast<uint32_t>(MAX_IMBALANCE))];
               int a_imb = min(it->bidShares/100-it->askShares/100,static_cast<uint32_t>(MAX_IMBALANCE));
               a_negativeStates[a_imb].stateFeatures.conditionalMeanMidPriceChange
               = (a_ODS.stateFeatures.conditionalMeanMidPriceChange*(static_cast<long double>(a_ODS.numberOfMidPriceChanges))+ midPriceChange)
                 /(static_cast<long double>(a_ODS.numberOfMidPriceChanges)+1.0);
               a_negativeStates[a_imb].stateFeatures.timeToMidPriceChange
               = (a_ODS.stateFeatures.timeToMidPriceChange*(static_cast<long double>(a_ODS.numberOfMidPriceChanges))+ midPriceChange)
                 /(static_cast<long double>(a_ODS.numberOfMidPriceChanges)+1.0);
	            a_negativeStates[a_imb].numberOfMidPriceChanges++;
       		}
   		else
    			{
               long double midPriceChange = static_cast<long double>(futureLevelOneBook.bidPrice+futureLevelOneBook.askPrice)/2.0-
                                            static_cast<long double>(it->bidPrice+it->askPrice)/2.0;
               long double timeToMidPriceChange = static_cast<long double>(a_futureIndex - a_index);
               oneDState a_ODS = a_balancedState;
               a_balancedState.stateFeatures.conditionalMeanMidPriceChange
               = (a_ODS.stateFeatures.conditionalMeanMidPriceChange*(static_cast<long double>(a_ODS.timeSpan))+ midPriceChange)
                 /(static_cast<long double>(a_ODS.numberOfMidPriceChanges)+1.0);
               a_balancedState.stateFeatures.timeToMidPriceChange
               = (a_ODS.stateFeatures.timeToMidPriceChange*(static_cast<long double>(a_ODS.numberOfMidPriceChanges))+ midPriceChange)
                 /(static_cast<long double>(a_ODS.numberOfMidPriceChanges)+1.0);
	            a_balancedState.numberOfMidPriceChanges++;
	   		}

        it++;
        a_index++;
     }
     a_closingConditionalBooks.erase(a_closingConditionalBooks.begin(), a_closingConditionalBooks.begin()+a_futureIndex);

   return;
}



#endif /* COMPUTE_ROUTINES_H_ */


