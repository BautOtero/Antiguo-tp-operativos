#Parametro 1 es el auth token de ngrok
cd ..
wget https://faq.utnso.com.ar/ngrok.zip
sudo apt install unzip
unzip ngrok.zip
./ngrok authtoken $1
git clone http://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library
sudo make install
cd ..
cd tp-2020-1c-Z-
cd Delibird
cd DelibirdCore
sudo make install
