#include <iostream>
#include <pqxx/pqxx>

int main() {
    try {
        pqxx::connection c(
            "host=aws-0-ca-central-1.pooler.supabase.com "
            "port=5432 "
            "dbname=postgres "
            "user=postgres.nbjzlzreuwryagkoqvgi "
            "password=COMP3400Project "
            "sslmode=require"
        );

        if (c.is_open()) {
            std::cout << "Connected to database successfully: " << c.dbname() << std::endl;
        } else {
            std::cerr << "Unable to open database" << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
