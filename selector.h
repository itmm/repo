#pragma once

class Selector {
    public:
        int handler { -1 };
        bool can_read { false };
        bool can_write { false };

        void close();
        ~Selector() { close(); }

        virtual void do_read();
        virtual void do_write();
};
