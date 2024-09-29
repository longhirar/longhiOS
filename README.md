# longhiOS

longhiOS é um sistema operacional hobby desenvolvido em C e Assembly para a arquitetura x86, utilizando o padrão Multiboot2 e o GRUB como carregador de boot. O projeto tem como objetivo servir como uma plataforma de aprendizado para desenvolvimento de sistemas, com foco em um kernel mínimo que gerencia memória e exibe informações básicas durante o boot.

## Funcionalidades

* Kernel mínimo com suporte básico ao gerenciamento de memória.
* Libc básica própria.
* Tela inicial que exibe todas as cores RGB e informações de boot (incluindo o mapa de memória).
* Compatível com Multiboot2 e BIOS.

## Requisitos

O projeto longhiOS requer as seguintes ferramentas para ser compilado e testado:

* QEMU (para testes, integrado com o projeto), ou qualquer máquina virtual x86 (sem integração).
* Sistema operacional Linux para a configuração do ambiente de desenvolvimento.

## Como começar

1. Clone o repositório:
    ```bash
    git clone https://github.com/seu-usuario/longhiOS.git
    ```
2. Execute o script de build:
    O script build.sh compilará os cross-compilers (GCC e Binutils) necessários e o sistema, configurando tudo automaticamente.
    ```bash
    ./build.sh
    ```

3. Teste no QEMU:
    Após a compilação, utilize o script qemu.sh para rodar o longhiOS em um ambiente de emulação QEMU.
    ```bash
    ./qemu.sh
    ```

4. Criar uma ISO:
    O script iso.sh pode ser usado para gerar uma imagem ISO que pode ser colocada em um CD ou pendrive para boot em hardware real.
    ```bash
    ./iso.sh
    ```

## Estrutura do Projeto

* Kernel: O núcleo do longhiOS, responsável pelo gerenciamento básico de memória e pela inicialização do sistema.
* Libc: Uma implementação básica da biblioteca C, com uma versão especial (LIBK) utilizada para o kernel.
* system.bin: Arquivo gerado após a compilação, compatível com Multiboot2, que pode ser carregado pelo GRUB.

## Funcionalidade Atual

No momento, o longhiOS exibe uma tela inicial com todas as cores RGB e informações sobre o boot, incluindo o mapa de memória detectado. Ainda não há suporte para interação com o usuário, pois drivers de entrada não foram implementados.
Futuro do Projeto

O foco do projeto é o aprendizado, e no momento ele não aceita contribuições externas. No entanto, o código está disponível para referência e aprendizado, e o desenvolvimento segue sob a licença MIT.

## Licença

Este projeto é licenciado sob a Licença MIT. Você pode ler ela no arquivo LICENSE.TXT