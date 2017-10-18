#!/bin/bash 

#Prima di tutto definisco variabili di environment 
#Di base queste 4 solitamente servono
exePATH="path/to/executable"
outDIR="$PWD/out"
errDIR="$PWD/err"
binDIR="$PWD/bin" 
QUEUE="background" 
TEST="TRUE" 

#pulisco la cartella dei binari da quelli vecchi 

rm ${binDIR}/*

#inizio un loop 
#$1 è la prima variabile che inserisci dopo lo script e.g. ./TestLancio.sh 100
for((i = 0; i < $1; i++));
	do
	#sleep di 0.1s per non mandare lo scheduler in overflow 
	sleep 0.1

	#definisco un nome del job 
	JOBNAME="TestScript_${i}"

	#iniziamo a creare un file eseguibile da sottomettere poi nel job
	echo "Sottometto il job #${i}"

	echo "#!/bin/bash" >> ${binDIR}/${JOBNAME}.sh

	#fare il source della propria bashrc può sempre essere utile 
	echo "source /path/to/your/home/.bashrc" >> ${binDIR}/${JOBNAME}.sh

	#se non ci sono altre operazioni preliminari, inseriamo la riga con l'eseguibile 
	echo "${exePATH}/your_executable -various_options" >> ${binDIR}/${JOBNAME}.sh 

	#rendo l'eseguibile tale 
	chmod +x ${binDIR}/${JOBNAME}.sh 

	#Se la variabile di test è uguale a TRUE lui stampa  quello che eseguirebbe, senza eseguirlo 
	if [ "${TEST}" == "TRUE" ] ; then
	#-q definisce la coda (puoi vedere le code su pbs con qstat -q 
        #-N da il nome al job 
	#-o definisce la posizione del file .out 
	#-e definisce la posizione del file .err 
 
	 echo "qsub -q $QUEUE -N job_$i -o $outDIR/job_$i.out -e $errDIR/job_$i.err ${binDIR}/${JOBNAME}.sh"

 	else 
	#altrimenti, sottomette 
	 qsub -q $QUEUE -N ${JOBNAME} -o ${outDIR}/${JOBNAME}.out -e ${errDIR}/${JOBNAME}.err ${binDIR}/${JOBNAME}.sh

 	fi

	
	done
