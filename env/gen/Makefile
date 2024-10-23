all:
	mkdir ./dist/ ; cp gen.py ./dist/gen

binary: gen.py
	~/.local/bin/pyinstaller --onefile gen.py

install:
	cp ./dist/gen /usr/bin/

uninstall:
	rm /usr/bin/gen

clean:
	rm -r build dist __pycache__ *.spec
