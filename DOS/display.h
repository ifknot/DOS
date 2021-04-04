#pragma once

namespace xt {

    /**
    * C98 Singleton pattern
    */
    class display {

    public:

        static display& instance();

    private:

        display() {}
        display(display const&);
        void operator=(display const&);

    };

}