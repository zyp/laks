class RBlog(gdb.Command):
	def __init__(self):
		gdb.Command.__init__(self, "rblog", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL, True)
	
	def invoke(self, arg, from_tty):
		args = gdb.string_to_argv(arg)
		if len(args) != 1:
			print 'Usage: rblog <object>'
			return
		
		rblog = gdb.parse_and_eval(args[0])
		num_entries = rblog['num_entries']
		num_arguments = rblog['num_arguments']
		entries = rblog['entries']
		index = rblog['index']
		
		if entries[index]['string']:
			r = range(index, num_entries) + range(index)
		else:
			r = range(index)
		
		for i in r:
			entry = entries[i]
			timestamp = int(entry['timestamp'])
			if not entry['string']:
				break
			string = entry['string'].string()
			arguments = [int(entry['arguments'][i]) for i in range(num_arguments)]
			
			try:
				string = string % tuple(arguments[:string.count('%') - 2 * string.count('%%')])
			except:
				pass
			
			print '%8d  %-80s  %s' % (timestamp, string, ' '.join('%08x' % a for a in arguments))

RBlog()
