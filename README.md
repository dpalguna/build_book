build_book
==========

Builds book from itch data. Computes state features from historical data.

Brief description of what the code does. 

1) build_book_uncross.cpp is the file that contains the main routine.

2) g++ build_book_uncross.cpp -std=c++11 -o build_book_uncross

3) ./build_book_uncross "data file for a single ticker" "time interval delta in ms"

4) class_definitions.h contains class definitions for the messages, state, features, timestamp

5) read_and_update.h reads a message and updates the book accordingly. For this, any executions that occur at a deeper level of the book or violates time priority would "kill" the orders whose priority got violated. These dead orders will get deleted from the book.

6) book_routines.h provides the functionality to update the book, like adding orders, deleting orders, printing the book, summarizing the book into level one data etc.


Next steps:
1) Predict using the features and compute error rates to make order placement decisions.
2) Implement the predictions into the execution strategy.
   

