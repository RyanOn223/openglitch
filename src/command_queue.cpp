#include "command_queue.h"

command_queue::command_queue()
{
    //ctor
}
void command_queue::push(const command& cmd)
{
    std::cout << "pushed command\n";
    cmd_queue.push(cmd);
}
command command_queue::pop()
{
    std::cout << "popped command\n";
    //not great but pop() is made all safe or whatever
    command cmd;
    cmd = cmd_queue.front();
    cmd_queue.pop();
    return cmd;
}
bool command_queue::is_empty() const
{
    return cmd_queue.empty();
}
command_queue::~command_queue()
{
    //dtor
}
