## Echipa tema 3 SD: 
    Constantin Dimitrie 315CA -> id Moodle : dimitrie.constantin
    Dinca Aelius-Gabriel 315CA 

## Titlu + numar temă
    Tema 3 - Social media

### Descriere:

Task 1 - friends.c
    - In friends.c am implementat functionalitati pentru gestionarea unei retele de prieteni intr o platforma de social media. Functionalitatile includ adaugarea unui prieten, inlaturarea unui prieten, afisarea sugestiilor de prietenie, afisarea distantei dintre doi useri, gasirea prietenilor comuni si identificarea celui mai popular user.
    - Din punctul de vedere al implementarii, am ales un graf neorientat, construit cu liste de adiacenta, in care stocam fiecare user in parte. Graful este initializat in fisierul social_media.c.
    - Descrierea functiilor:
        -> add_friend : Functia primeste ca parametri graful userilor si id-urile celor doi useri, urmand sa se adauge legaturile aferente in graf (se adauga doua legaturi pentru ca graful sa fie orientat)
        -> remove_friend : Functia este similara cu add_friend, singura diferenta fiind ca se elimina legaturile din graf
        -> user_suggestions : Functia primeste ca parametri graful user-ilor si numele user-ului pentru care se efectueaza sugestiile. Fiecare sugestie este o relatie de prietenie de ordin 2, adica un prieten al unui prieten al user-ului, deci se calculeaza vectorul de distante dintre fiecare user prin apelul unei functii care efectueaza un Breadth First Search (BFS) pentru graful user-ilor. La final, se parcurge vectorul de distante si se afiseaza doar userii care au distanta 2 fata de user-ul trimis ca parametru.
        -> find_distance : Functia primeste ca parametri graful user-ilor si id-urile celor doi useri. Ca in cazul anterior, se calculeaza vectorul de distante printr-un BFS si se verifica distanta dintre cei doi useri.
        -> find_common_friends : Functia primeste ca parametri graful user-ilor si id-urile celor doi useri. Pentru a afla prietenii comuni, se parcurg listele de prieteni ale celor doi useri. Pentru fiecare prieten al user-ului 1, se verifica daca exista in lista de prieteni a user-ului 2. Fiecare prieten comun este stocat intr-un buffer auxiliar, care este sortat la final in ordinea crescatoare a id-urilor.
        -> find_the_popular_one : Functia primeste ca parametri graful user-ilor si numele user-ului pentru care se efectueaza cautarea. Pentru a afla cel mai popular prieten, se parcurge lista de prieteni si se gaseste numarul maxim de prieteni.
        -> handle_input_friends : Functia se ocupa cu parsarea inputului si apelarea functiilor aferente

Task 2 - posts.c
    - In posts.c am implementat functionalitati pentru gestionarea postarilor si a repostarilor intr-o platforma de social media. Functionalitatile includ crearea/stergerea unei postari/repostari, adaugarea de like-uri la o postare/repostare, afisarea tuturor repostarilor unui user, gasirea primului repost comun, afisarea numarului de like-uri si efectuarea operatiei de ratio.
    - Din punctul de vedere al implementarii, am utilizat un vector de postari/repostari (fiind eficient din punctul de vedere al eficientei accesarii). Fiecare postare/repostare este adaugata in vector dupa id. Postarile se diferentiaza de repostari prin existenta continutului postarii si prin parent id, care este initializat cu 0, deoarece nu exista user id-ul 0.
    - Descrierea functiilor: 
        -> create_post : Functia primeste ca parametri numarul postarilor, vectorul de postari, numele user-ului si titlul postarii. Daca exista titlul, inseamna ca se adauga o postare. Se creeaza o noua intrare in vector si se incrementeaza numarul de postari.

        -> repost : Functia primeste ca parametri numarul postarilor, vectorul de postari, id-ul postarii parinte si id-ul repostarii parinte, daca este cazul. Se seteaza parent id-ul repostarii si se adauga repostarea in graful postarii parinte.

        -> get_reposts : Functia primeste ca parametri vectorul de postari, id-ul postarii si eventual id-ul repostarii. Daca id-ul repostarii este 0, se considera cazul repostarii. La final se efectueaza un Depth First Search (DFS) si se afiseaza postarile.

        -> delete : Functia primeste ca parametri vectorul de postari, id-ul postarii si eventual id-ul repostarii, urmand sa se elimine intrarea asociata id-ului din vector, inclusiv legaturile din arbori.

        -> common_repost : Functia primeste ca parametri vectorul de postari, id-ul postarii si id-urile repostarilor. Functia parcurge arborele de repostari incepand cu repostarea cu id1 , incrementand height1 la fiecare pas pana ajunge la postarea parinte. Se face acelasi lucru si pentru repostarea cu id2. Daca height1 este mai mic decat height2, inseamna ca postarea id1 este mai aproape de postarea originala decat postarea id2. In acest caz, se parcurge arborele de reposturi incepand cu postarea id2, coborand cu height2 - height1 pasi pentru a ajunge la acelasi nivel cu postarea id1. Daca height1 este mai mare sau egal cu height2, se face acelasi lucru, dar incepand cu postarea id1. In acest punct, id1 si id2 sunt la acelasi nivel in arborele de reposturi. Se parcurge arborele de reposturi in sus incepand cu ambele postari, pana cand se intalnesc. Acest punct de intalnire este primul repost comun intre cele doua postari.

        -> like_the_post : Functia primeste ca parametri vectorul de postari, id-ul postarii, numele user-ului si eventual id-ul repostarii. Se itereaza lista like-urilor si se cauta daca postarea a primit deja like de la acel user. In acest caz, se elimina like-ul , iar , in caz contrar, se adauga o noua intrare in lista.

        -> get_likes : Functia primeste ca parametri vectorul de postari, id-ul postarii si eventual id-ul repostarii. Se afiseaza dimensiunea listei in care sunt stocate like-urile.

        -> ratio : Functia primeste ca parametri vectorul de postari, id-ul postarii si numarul total de postari. Se parcurge vectorul de postari si se gaseste numarul maxim de like-uri al repostarilor asociate postarii initiale. Daca numarul maxim de like-uri este mai mic decat cel al postarii, inseamna ca user-ul a evitat sa primeasca 'ratio'.

        -> handle_input_posts : Functia se ocupa cu parsarea inputului si apelarea functiilor aferente

Task 3 - feed.c
    - In feed .c am implementat functionalitati care combina cele implementate pana acum, fiind varianta finala a functionalitatii unui social media. 
    - Din punctul de vedere al implementarii, am folosit structurile de date implementate la task-urile anterioare.
    - Descrierea functiilor: 
        -> get_feed : Functia primeste ca parametri vectorul de postari, graful de useri, numele user-ului, marimea feed-ului cerut si numarul total de postari. Se parcurge vectorul de postari si se verifica daca postarea/repostarea de pe pozitia curenta este asociata user-ului sau al unui prieten de-al lui. In acest caz, se afiseaza postarea/repostarea si se decrementeaza feed_size.

        -> view_profile : Functia primeste ca parametri vectorul de postari, graful de useri, numele user-ului si numarul total de postari. Se parcurge vectorul de postari si se afiseaza postarile care apartin user-ului.

        -> friends_repost : Functia primeste ca parametri vectorul de postari, graful de useri, numele user-ului, id-ul postarii si numarul total de postari. Se parcurge vectorul de postari si se afiseaza numele prietenilor care au dat repost.

        -> common_group : Functia primeste ca parametri graful de useri si id-ul userului pentru care se cauta clica maximala. Functia initializeaza mai intai patru vectori R, P, X. Vectorii R, P si X sunt folositi in algoritmul Bron-Kerbosch pentru a gasi clicile dintr-un graf. Vectorul rmax va stoca cea mai mare clica gasita, iar rmaxsize va stoca dimensiunea acesteia. Variabila nod este folosita pentru a parcurge lista de vecini a utilizatorului. Utilizatorul este adaugat in vectorul P si toti vecinii sai sunt adaugati in vectorul P. Apoi, se aplica algoritmul Bron-Kerbosch pe graful de utilizatori. Algoritmul Bron-Kerbosch a fost implementat folosind ca documentatie site-ul "https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#Without_pivoting".
    

