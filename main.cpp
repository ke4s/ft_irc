#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>


/*

İki bilgisayar arası haberleşme için her iki bilgisayarında soket yolunu socket fonksiyonu ile açması gerekir.

 int socket(int domain, int type, int protocol);
    Domain parametresi AF_UNIX (Unix dosyası), AF_INET (IPv4), AF_INET6 (IPv6)
    Type parametresi ise SOCK_STREAM (TCP), SOCK_DGRAM (UDP), SOCK_RAW sabitlerini alır.
    Protocol parametresi de genellikle 0 değerini verilir. 0 default olarak sistemdekini çeker IPPROTO_TCP = TCP transport protocol


Sunucu veya alıcı temel olarak belirlenen port adresine gelen istekleri dinler ve kabul eder.

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);



    int accept(int sockfd, struct sockaddr *addr , socklen_t *addrlen);
    Fonksiyonun ilk parametresi socket fonksiyonu ile oluşturulan değeri, ikinci parametresi bağlanacak istemcinin bilgilerinin tutulduğu veri yapısını, son parametre ise bu bilgilerin boyutunu alır.
    Burada biraz kafalar karışabilir.
    Çünkü accept fonksiyonu socket fonksiyonu gibi yeni bir yol oluşturur.
    Bu yol istemciye ulaşan bir yoldur ve socket fonksiyonu ile oluşturulan yol ile karıştırılmamalıdır.



ssize_t recv(int sockfd, void *buffer, size_t length, int flags);
ssize_t send(int sockfd, const void *buffer, size_t length, int flags);
Fonksiyonların ilk parametresi verinin gönderileceği ve alınacağı soket adresi (accept ile alınan), ikinci parametreleri gönderilen veya alınan veriyi, üçüncü parametre ise veri boyutunu alır.

Son parametre ise veri gönderimi sırasında ve alımı sırasında verinin nasıl gönderileceği ve alınacağı, işlem yapılıp yapılmayacağı gibi çeşitli sabit değerleri alır bu alana genellikle 0 değeri verilir.

Tüm işlemlerden sonra dosya işlemlerinde olduğu gibi soketin kapatılması gerekir.
------------------------------------

. İlk bilmeniz gereken: iki tür byte sıralaması vardır: 
en önemli baytın (ki buna bazen öktet de denir) önce geldiği sıralama veya en önemli baytın sonra geldiği sıralama. 
Bu sıralamalardan birincisine "Ağ Bayt Sıralaması" (Network Byte Order) denir. 

Bir şeyin Ağ Bayt Sıralaması'na göre sıralanması gerektiğini söylediğimde htons() gibi bir işlev çağırmanız gerekecek ("Konak Bayt Sıralaması"ndan [Host Byte Order] "Ağ Bayt Sıralaması"na dönüştürebilmek için).
Eğer "Ağ Bayt Sıralaması"ndan bahsetmiyorsam o zaman ilgili veriyi olduğu gibi yani "Konak Bayt Sıralaması" düzeninde bırakmanız gerekir.



Bu veri yapısı pek çok türde soket için soket adres bilgisini barındırır:
struct sockaddr {
    unsigned short    sa_family;    // adres ailesi, AF_xxx
    char              sa_data[14];  // protokol adresinin 14 byte'ı
};

lakin sa_data'yı doldurmak uğraştırıcı olduğu için sockaddr'ye paralel bir yapı tasarlamışlar:
struct sockaddr_in {
    short int          sin_family;  // Adres ailesi. Sadece işletim sistemi çekirdeği tarafından veri yapısının barındırdığı adresin türünü tespit etmek için kullanılır. Bu yüzden de bu alanın Konak Bayt Sıralamasında bırakılması gerekir. 
    unsigned short int sin_port;    // Port numarası. UDP/TCP katmanında paketlenir. Bu yüzden Ağ Bayt Sıralamasında gönderilmeleri gerekir
    struct in_addr     sin_addr;    // Internet adresi. IP katmanında paketlenir. Bu yüzden Ağ Bayt Sıralamasında gönderilmeleri gerekir
    unsigned char      sin_zero[8]; // struct sockaddr ile aynı boyda olması için kullanılan yer kaplama değişkeni. İçinin komple 0 olması gerekmekte. UPCAST türevi bir işlem yapılırken iki struct'ın da boyutu aynı olmalı.
};

Buna ek ve daha da önemli olarak bir struct sockaddr_in göstergesi struct sockaddr göstergesine dönüştürülebilir ve tersi de doğrudur.
 Yani her ne kadar socket() işlevi struct sockaddr* şeklinde bir veri beklese de siz gene de struct sockaddr_in kullanıp son anda gerekli dönüştürmeyi yapabilirsiniz! 
 Ayrıca sin_family değişkeninin de struct sockaddr yapısındaki sa_family değişkenine karşılık geldiğini ve "AF_INET" olarak ayarlanması gerektiğini unutmayın. Son olarak sin_port ve sin_addr değikenlerinin de Ağ Byte Sırasında bulunmaları gerektiğini unutmayın!

// Internet adresi (tarihi sebeplerden ötürü korunmakta)
struct in_addr {
    unsigned long s_addr; // 32-bit yani 4 bytes
};


 Ağ ve Konak Bayt Sıralamaları

htons() -- "Host to Network Short" -- konaktan ağa short
htonl() -- "Host to Network Long" -- konaktan ağa long
ntohs() -- "Network to Host Short" -- ağdan konağa short
ntohl() -- "Network to Host Long" -- ağdan konağa long


unsigned long  inet_addr(char *ip)  Ağ Bayt Sıralamasına göre dizilmiş olarak döndürür yani htonl() kullanmaya gerek yok. Hata denetimi yoktur(tehlikeli). UL dönüş değeri -1 geldiğini varsayarksak 255.255.255.255 gelir. Kendi çeviricimizi yazmamaız mantıklı
inet_addr("10.12.110.57")


 bind() -- Hangi Port Üzerindeyim?
 	int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
	Bir soket edindikten sonra bunu makinanızdaki bir "port" ile ilişkilendirmek isteyeceksiniz.
 	Bu port numarası dediğimiz şey işletim sistemi çekirdeği tarafından gelen bir paketi belli bir sürecin soket tanımlayıcısı ile ilişkilendirebilmesi için gereklidir.
 	Eğer tek yapacağınız bir yere connect() ile bağlanmaksa o zaman buna gerek yoktur.


 connect()--Hey, sen!
 int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
	kullanıcı sizden "10.12.110.57" adresine ve "23" numaralı porta bağlanmanızı istiyor (standart telnet portu). Aha! Şimdi ne yapacaksınız?
	 sockfd dediğimiz değişken socket() sistem çağrısı tarafından döndürülmüş olan soket dosya tanımlayıcısının değerini tutar. serv_addr ise struct sockaddr türünde bir değişkendir ve hedef port ile IP adres bilgilerini barındırır.
	 addrlen değişkeni de sizeof(struct sockaddr) değerini alırsa iyi olur.
	 Yerel port numarası ile ilgilenmiyoruz; bizi ilgilendiren hedef bilgisayar (uzaktaki port). İşletim sistemi çekirdeği bizim için bir yerel port bulacaktır ve bağlandığımız bilgisayar da bu bilgiyi otomatik olarak bizden öğrenecektir. Siz tatlı canınızı üzmeyin.



listen() -- Biri Beni Arayabilir mi Acaba?
int listen(int sockfd, int backlog);
	size gelen bağlantıları dinlemek ve bunlarla ilgili gerekeni yapmak istediğinizi var sayalım. Bu tür bir süreç iki aşamalıdır, önce listen() ile dinler sonra da accept() ile gelen çağrıları kabul edersiniz.
	backlog ise gelen çağrı kuyruğunda izin verilen bağlantı sayısını gösteriyor. Bu ne mi demek? Şey, yani gelen bağlantı talepleri siz onları accept() ile kabul edene dek bir kuyrukta bekler. Genelde maks 20dir

accept() -- "3490 Numaralı Portu Aradığınız İçin Teşekkürler"
int accept(int sockfd, void *addr, int *addrlen);
 Siz accept() işlevini çağırırsınız ve ona beklemekte olan çağrıyı kabul etmesini söylersiniz. O da size yepyeni bir soket dosya tanımlayıcısı döndürür sadece ve sadece söz konusu bağlantıya özel.
 addr  struct sockaddr_in structı dır ve kabul ettiğiniz anda struct'ın içi bağlanan bilgisayarın bilgileri ile dolacak.


 send() ve recv() -- Konuş Benimle Bebeğim!
 int send(int sockfd, const void *msg, int len, int flags);
	ockfd üzerinden veri göndereceğiniz sokettir. msg göndermek istediğiniz mesajı gösteren bir göstergedir ve len değişkeni bu verinin byte cinsinden uzunluğudur.
	flags parametresini 0 olarak bırakabilirsiniz. (Bu parametre ile ilgili ayrıntılı bilgi için bkz. send() man sayfası.) send() değer olarak gönderilen bayt miktarını döndü
 int recv(int sockfd, void *buf, int len, unsigned int flags);

 
*/




int main(int argc, char *argv[])
{

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server_attr;
    memset(&server_attr, 0, sizeof(server_attr));

    //server bilgilerini doldurma
    server_attr.sin_family = AF_INET; //ipv4
    server_attr.sin_addr.s_addr =  INADDR_ANY; //bu tanım bulunduğu bilgisayarın ip adresini almasını söyler //inet_addr("127.0.0.1"); // htonl(INADDR_ANY)  INADDR_ANY sabitinin mesela 12 olarak belirlendiği durumlarda işe yarar.
    server_attr.sin_port = htons(1453);


    //Adres bağlama
    if (bind(server_sock, reinterpret_cast<struct sockaddr *>(&server_attr), sizeof(server_attr)) < 0)
    {
        std::cout << "ERROR binding" << std::endl;
        return -1;
    }
    else
        std::cout << "Binding succesfull" << std::endl;


    //Server socket'imize gelen istekleri dinleneme
    if (listen(server_sock, 3) < 0)
    {
        std::cout << "Dinleme sırasında bir hata oluştu." << std::endl;
        return -1;
    }

    //Client bilgilerini tutacak değişken oluştur ve accept ile değişkeni doldur.
    struct sockaddr_in  client_attr;
    socklen_t           size_client  = sizeof(client_attr);
    int                 client_sock = accept(server_sock, reinterpret_cast<struct sockaddr *>(&client_attr), &size_client); //client_sock yerine client_fd demek daha doğru olabilir.
    
    if (client_sock < 0)
    {
        std::cout << " ACCEPT ERROR" << std::endl;
        return -1;
    }
    else
        std::cout << "ACCEPT SUCCES" << std::endl;

    const char * mesaj = "Bağlantı mesajı geldi";
    if (send(client_sock, mesaj, strlen(mesaj), 0) < 0)
    {
        std::cout << "Gönderilmedi " << std::endl;
        return -1;
    }

    while(1)
    {
        int err;
        char *buff = (char *)malloc(512);
        if ((err = recv(client_sock, buff, 512, 0)) == 0)
        {   
            close(client_sock);
            //client bağlantıyı kestiği için client socket ini kapat ve bilgilerini sil.
            std::cout << "bağlantı kesildi" << std::endl;
            //client çıktığında recive buraya girer.
        }
        else if (err < 0)
        {
            std::cout << "recive hata oluştu" << std::endl;
        }
        else
        {
            std::cout << "Gelen BUFF: " << buff << std::endl;
            if (send(client_sock, buff, strlen(buff), 0) < 0)
            {
                std::cout << "Gönderilmedi " << std::endl;
            }
        }
        std::cout << "donuyo" << err << std::endl;;
    }

    return 0;
}

