//
// Created by danie on 3/13/2025.
//

#ifndef BILL_H
#define BILL_H

class Bill {
    int cost;
    int due;
    int customer_id;

    public:
    Bill(int, int);
    void payCost(int);
    int getDue()const;
};



#endif //BILL_H
