FROM kible/proto:x86

WORKDIR /root/working

COPY . /root/working

RUN make -C rana_core_utils
RUN make -C rana

RUN mv /root/working/rana/output/* ./
RUN sed -i 's@src=@src=/kib/@g' /root/working/index.html

CMD [ "tail", "-f", "/dev/null" ]