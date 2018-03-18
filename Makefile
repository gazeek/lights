_lights.so: lights.c queue/queue.c queue/queue.h setup.py
	python setup.py build
	@ echo "copying $@ from 'build/....'."
	cp build/lib.*/$@ ./

