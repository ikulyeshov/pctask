all: app svr

app:
	make -C infra
	make -C main
	
svr:
	make -C server

clean:
	make -C main clean
	make -C infra clean
	make -C server clean