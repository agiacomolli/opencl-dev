====================
Instalação do OpenCL
====================

OpenCL (Open Computing Language) é um framework para escrita de programas que
executam em plataformas heterogêneas que consistem de CPUs, GPUs, DSPs, FPGAs e
outros processadores ou plataformas de computação em hardware.

OpenCL é um padrão aberto mantido pelo consórcio Khronos Group. O consórcio é
formado por diversos fabricantes, o que garante uma camada de compatibilidade
entre diferentes hardwares computacionais.


Instalação no Linux
===================

Vamos assumir uma distibuição baseada em Debian com gerenciador de pacotes
``apt``.  Os pacotes de desenvolvimento para OpenCL são instalados com o
comando::

    $ sudo apt install ocl-icd-opencl-dev

O pacote ``ocl-icd-opencl-dev`` contém as bibliotecas genéricas e arquivos
de cabeçalho para utilização das ferramentas de computação paralela.

Também é interessante instalar o software ``clinfo`` para verificar as
plataformas computacionais disponíveis. O pacote pode ser instalado com o
comando::

    $ sudo apt install clinfo

O programa ``clinfo`` lista as plataformas computacionais disponíveis no
sistema como segue::

    $ clinfo
    Number of platforms                               2

Caso nenhuma plataforma for encontrada, precisamos verificar quais dispositivos
necessitam de drivers adicionais. O OpenCL oferece suporte para computação
paralela em CPU e GPU. No caso de CPU, podemos escolher entre suporte para
processadores da Intel ou AMD. Encontramos o suporte para processadores da
Intel no pacote::

    $ sudo apt install beignet-opencl-icd

e para processadores da AMD em::

    $ sudo apt install boinc-client-opencl

Escolha qual dos pacotes deve ser instalado conforme o processador disponível.

Após a instalação dos pacotes, podemos executar novamente o comando
``clist`` e o número de plataformas deve ser atualizado::

    $ clinfo -l
    Platform #1: Intel Gen OCL Driver
     `-- Device #0: Intel(R) HD Graphics Skylake ULT GT2

Caso o sistema tenha placa de vídeo, podemos instalar também o driver para o
dispositivo. No caso de GPU Nvidia, os drivers podem ser instalador com o
comando::

    $ sudo apt install nvidia-opencl-dev

Listando novamente as plataformas computacionais com ``clist`` observamos
que uma nova plataforma é adicionada::

    $ clinfo -l
    Platform #0: NVIDIA CUDA
     `-- Device #0: GeForce 930M
    Platform #1: Intel Gen OCL Driver
     `-- Device #0: Intel(R) HD Graphics Skylake ULT GT2


Instalação no Windows
=====================

O Windows já deve possuir os drivers de dispositivo instalados nativamente. Os
pacotes são adicionados automaticamente quando os drivers de processador e
placa de vídeo forem instalados. Caso não sejam encontrados, é necessária a
atualização dos drivers, que pode ser feita via gerenciador de dispositivos.

Uma verificação simples pode ser feita com a busca pelo arquivo ``OpenCL.dll``.
O arquivo deve estar localizado em ``C:\Windows\System32\OpenCL.dll`` ou
``C:\Windows\SysWOW64``. Caso o suporte ao OpenCL não estiver disponível, a
busca não retornará nenhum resultado.

