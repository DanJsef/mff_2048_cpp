FROM gcc:12.1.0

RUN apt-get update && apt-get install -y cmake libncurses5-dev libncursesw5-dev

WORKDIR /app

COPY . .

RUN cmake .
RUN make

CMD ["./2048"]
