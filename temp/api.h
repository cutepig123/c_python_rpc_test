id sm_malloc(name, size_t n);
void* sm_get(id)
void sm_free(id);

class sm_obj{
	int id() const;
	void *data();
};

sm_obj* sm_malloc(size_t n);
void sm_free(sm_obj* );

class sm_obj_buffer{
	size_t x,y,step;
	char data[0];
};

sm_obj_buffer* sm_malloc_buffer(IMG_SIZE);
void sm_free_buffer(sm_obj_buffer* );
