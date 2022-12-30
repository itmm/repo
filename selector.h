#pragma once

class Selector_Handler;

class Selector {
    public:
        int handle {-1 };
        bool can_read { false };
        bool can_write { false };

        void close();
        ~Selector() { close(); }

        virtual void do_read(Selector_Handler &handler);
        virtual void do_write(Selector_Handler &handler);
};
