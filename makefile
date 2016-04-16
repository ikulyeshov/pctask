all: app svr

app:
	make -C cube
	make -C infra
	make -C main
	
svr:
	make -C cube
	make -C infra
	make -C server

clean:
	make -C cube clean
	make -C main clean
	make -C infra clean
	make -C server clean
