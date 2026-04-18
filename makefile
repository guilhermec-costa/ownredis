clean:
	@rm -rf server/bin
	@rm -rf cli/bin

cpl_cmds_server:
	bear --output server/compile_commands.json -- make -C server

cpl_cmds_cli:
	bear --output cli/compile_commands.json -- make -C cli