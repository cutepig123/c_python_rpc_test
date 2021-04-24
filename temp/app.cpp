
void client()
{
	sm_obj_buffer* buffer_obj = sm_malloc_buffer(buffer_size);
	Grab(buffer_obj->data());
	
	// insp
	cmd = sm_malloc();
	cmd.buffer = buffer_obj->id();
	send(cmd);
	recv();

	sm_freebuffer_(buffer_obj);
}
