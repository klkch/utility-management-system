//
// Created by danie on 3/13/2025.
//

#ifndef BILL_H
#define BILL_H

class Bill {
    int cost;
    int due;
    int provider_id;

    public:
    Bill(int, int);
    void payCost(int);
};



#endif //BILL_H
