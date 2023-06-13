FROM kible/proto:x86

WORKDIR /root/working

COPY . /root/working

RUN make -C rana_core_utils
RUN make -C rana
RUN sed -i 's@src=@src=/kib/@g' /root/working/rana/output/index.html

CMD [ "bash", "./launch.sh" ]