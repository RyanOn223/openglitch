#include "command_queue.h"

command_queue::command_queue()
{
    //ctor
}
void command_queue::push(const command& cmd)
{
    cmd_queue.push(cmd);
}
command command_queue::pop()
{
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
