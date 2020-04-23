FROM anothertest/nlex:latest

RUN pacman -S --noconfirm nodejs npm

ADD editor.html /editor.html
ADD res /res
ADD server /app

WORKDIR /app

RUN npm i mongodb express express-zip

ENV MONGO_ADDRESS "mongodb+srv://test:baloney1@cluster0-brvbk.mongodb.net"

ENTRYPOINT ["node", "out.js"]
