static void
addToZombie(Task * task)
{
   prepend_List(&zombieTasks, task);
   task->state = Task_zombie;
}
