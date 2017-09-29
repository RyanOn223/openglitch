#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H
#include "include.h"
#include "command.h"

class command_queue
{
    public:
        command_queue();
        virtual ~command_queue();
        void push(const command& cmd);
        command pop();
        bool is_empty() const;
    protected:

    private:
    std::queue<command> cmd_queue;
};

#endif // COMMAND_QUEUE_H
