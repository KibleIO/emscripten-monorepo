FROM kible/proto:x86

WORKDIR /root/working

COPY . /root/working

RUN make -C rana_core_utils
RUN make -C rana

CMD [ "bash", "./launch.sh" ]