FROM emscripten/emsdk

WORKDIR /root

COPY . /root

RUN make -C rana_core_utils
RUN make -C rana

RUN mv /root/rana/output/* ./
RUN sed -i 's@src=@src=/kib/@g' /root/index.html

ENTRYPOINT ["tail -f /dev/null"]