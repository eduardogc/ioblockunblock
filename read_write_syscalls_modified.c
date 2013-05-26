ssize_t vfs_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
  struct path path;
	char tmp[4096];
	char *pathname;
	int pathSize;
	int i, j;
	int blocked = 0;
	extern char ioBlock[128][256];
	ssize_t ret;

	path = file->f_path;	
	pathname = d_path(&path, tmp, 4096);	
	
	pathSize = strlen(pathname);
	if (pathSize > 0)
	{		
		for (i = 1; i <= ioBlock[0][0]; ++i)
		{
			if (blocked) break;
			
			j = 0;
			if ( pathSize >= strlen(ioBlock[i]) )			
				for ( ; j < strlen(ioBlock[i]); ++j)				
					if (pathname[j] != ioBlock[i][j])						
						break;					
			
			if (strlen(ioBlock[i]) > 0 && j == strlen(ioBlock[i]))
				if (pathname[j] == '/' || pathname[j] == '\0')
					blocked = 1;			
		}		
	}
	
	if (blocked)	
		return -1;		

	if (!(file->f_mode & FMODE_READ))
		return -EBADF;
	if (!file->f_op || (!file->f_op->read && !file->f_op->aio_read))
		return -EINVAL;
	if (unlikely(!access_ok(VERIFY_WRITE, buf, count)))
		return -EFAULT;

	ret = rw_verify_area(READ, file, pos, count);
	if (ret >= 0) {
		count = ret;
		if (file->f_op->read)
			ret = file->f_op->read(file, buf, count, pos);
		else
			ret = do_sync_read(file, buf, count, pos);
		if (ret > 0) {
			fsnotify_access(file);
			add_rchar(current, ret);
		}
		inc_syscr(current);
	}

	return ret;
}


ssize_t vfs_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
  struct path path;
	char tmp[4096];
	char *pathname;
	int pathSize;
	int i, j;
	int blocked = 0;
	extern char ioBlock[128][256];
	ssize_t ret;

	path = file->f_path;		
	pathname = d_path(&path, tmp, 4096);	
	
	pathSize = strlen(pathname);
	if (pathSize > 0)
	{		
		for (i = 1; i <= ioBlock[0][0]; ++i)
		{
			if (blocked) break;
			
			j = 0;
			if ( pathSize >= strlen(ioBlock[i]) )			
				for ( ; j < strlen(ioBlock[i]); ++j)				
					if (pathname[j] != ioBlock[i][j])						
						break;					
			
			if (strlen(ioBlock[i]) > 0 && j == strlen(ioBlock[i]))
				if (pathname[j] == '/' || pathname[j] == '\0')
					blocked = 1;			
		}		
	}
	
	if (blocked)	
		return -1;	
	
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!file->f_op || (!file->f_op->write && !file->f_op->aio_write))
		return -EINVAL;
	if (unlikely(!access_ok(VERIFY_READ, buf, count)))
		return -EFAULT;

	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret >= 0) {
		count = ret;
		if (file->f_op->write)
			ret = file->f_op->write(file, buf, count, pos);
		else
			ret = do_sync_write(file, buf, count, pos);
		if (ret > 0) {
			fsnotify_modify(file);
			add_wchar(current, ret);
		}
		inc_syscw(current);
	}

	return ret;
}


