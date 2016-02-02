DANIELE VERRIENTI					HWC2 454949
Il seguente homework ha come obiettivo principale quello di simulare il funzionamento di un dispatcher che, dato un insieme di input generati da un flusso provider, smista i suddetti messaggi ad un insieme di reader ed, eventualmente, elimina quei reader che, a causa della loro lentezza nel processamento dei dati, possono portare ritardi all'interno dell'intera architettura.
Il progetto prevede l'implementazione di un provider che,mediante la funzione create_message_sequence,crea e immette all'interno di un buffer circolare, condiviso con il dispatcher, una sequenza di messaggi(rappresentati come una struct messaggi*) e, prima di terminare, genera una poison pill.
Il dispatcher presenta una funzione “dispatch_message(int max)”, dove per max si è supposto un massimo tempo di processamento ammesso dal dispatcher e viene settato a valori diversi nei test per coprire il caso di reader veloci e lenti. In tale funzione è presente un meccanismo di confronto tra il parametro max e il parametro tempo processamento del reader e l'invocazione della funzione reader_list_delete_reader(max) sulla reader list,   a cui spetta il compito di eliminare quei reader con tempo processamento > max.
La reader list è una lista di reader(dove ogni reader ha associato un nome, un tempo processamento e un buffer) con annesse funzioni di inserimento, cancellazione e modifica e a cui è stato associato un mutex per evitare interferenza. 
I reader vengono aggiunti alla reader list dall'accepter che, dopo aver estratto opportune richieste dal'accepter buffer, crea un'oggetto reader e lo aggiunge alla lista. 
Tutti i buffer sono stati implentati utilizzando l'approccio seguito da Hwc1, ossia buffer circolare con indici per estrazione ed inserimenti ed appositi mutex per evitare interferenza nell'inserimento di messaggi.
Per ultimo abbiamo un flusso main con funzione “void fde_main()” che :
	1) inizializza tutti i buffer e tutti i flussi(dispatcher, accepter, provider etc..) sovracitati
	3) crea una lista con sei reader
	2) gestisce la terminazione dei suddetti flussi.
La classe TestDriver.c richiama tutti i test creati e presenti nella sottocartella del progetto  test e termina lanciando la funzione fde_main()(aspettare alcuni secondi dopo la stampa a video del messaggio “Partenza del dispatcher”). 
Per compilare e generare l'eseguibile è stato usato il comando:

gcc -g accepter.c accepter_buffer.c buffer.c dispatcher.c list.c message.c main.c poison_pill.c provider.c provider_buffer.c reader.c reader_buffer.c reader_list.c TestDriver.c ./test/*.c -lcunit -lpthread -o "HWC2"
