<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
	<meta name="generator" content="LibreOffice 5.1.6.2 (Linux)"/>
	<meta name="created" content="00:00:00"/>
	<meta name="changed" content="2017-06-30T04:47:46.506772633"/>	
</head>
<body lang="pt-BR" dir="ltr">
<h2 class="western">Memória compartilhada distribuída</h2>
<ul>
	<li/>
<p>O serviço amplia o tamanho do espaço de endereçamento de
	um sistema ao utilizar múltiplos servidores.</p>
	<li/>
<p>Cada servidor gerencia uma porção de memória local, que
	corresponde a uma parcela da memória total, que é dada pela soma
	das memórias de todos os servidores</p>
	<li/>
<p>Clientes podem ler ou escrever conteúdo na memória
	através das operações: 
	</p>
	<ul>
		<p><i>Escreve(Posicao1,buffer, tam_buffer)</i></p>
		<p><i>Le(Posicao4,tamanho)</i></p>
	</ul>
</ul>
<h2 class="western">Backup logger</h2>
<ul>
	<li/>
<p>Um serviço de log (executando em processo diferente do
	serviço de memória) deve salvar cópias do estado da memória
	global em disco.</p>
	<li/>
<p>Em cada máquina que disponibiliza um segmento de memória
	para o serviço existe um processo logger.</p>
	<li/>
<p>Um dos processos logger (escolhido arbitrariamente)
	solicita aos demais loggers o conteúdo das suas memórias e escreve
	um arquivo consolidado com o conteúdo da memória global naquele
	instante.</p>
</ul>
<h2 class="western">Modo de compilar/executar as aplicações:</h2>
<p><b>#server1.c</b></p>
<p><i>gcc -o server server1.c -pthread &amp;&amp; ./server </i><font color="#ff3333"><i>&lt;TAMANHO&gt;
&lt;ID&gt; &lt;PORT&gt;</i></font></p>
<p><b>#client1.c</b></p>
<p><i>gcc -o client client1.c &amp;&amp; ./client</i></p>
<p><b>#logger.c</b></p>
<p><i>gcc -o logger logger.c &amp;&amp; ./logger <font color="#ff3333">&lt;TAMANHO&gt;
&lt;ID&gt; &lt;PORT&gt;</font></i></p>
<p><b>#loggerm.c</b></p>
<p><i>gcc -o loggerm loggerm.c &amp;&amp; ./loggerm <font color="#ff3333">&lt;TEMPO&gt;</font></i></p>
<p><b>#conf.txt</b></p>
<p><font color="#ff3333"><i>&lt;TAMANHO&gt;</i></font></p>
<p><i><font color="#ff3333">&lt;IP1&gt;</font>:<font color="#ff3333">&lt;PORT1&gt;</font></i></p>
<p><i><font color="#ff3333">&lt;IP2&gt;</font>:<font color="#ff3333">&lt;PORT2&gt;</font></i></p>
<p>     <i>:</i></p>
<p><i><font color="#ff3333">&lt;IPn&gt;</font>:<font color="#ff3333">&lt;PORTn&gt;</font></i></p>
<p><b>#conf2.txt</b></p>
<p><font color="#ff3333"><i>&lt;TAMANHO&gt;</i></font></p>
<p><i><font color="#ff3333">&lt;IP1&gt;</font>:<font color="#ff3333">&lt;PORT1&gt;</font></i></p>
<p><i><font color="#ff3333">&lt;IP2&gt;</font>:<font color="#ff3333">&lt;PORT2&gt;</font></i></p>
<p>     <i>:</i></p>
<p><i><font color="#ff3333">&lt;Ipn&gt;</font>:<font color="#ff3333">&lt;PORTn&gt;</font></i></p>
<p><br/>
</p>
<h2>Para mais informações, contate:</h2>
<p><b>Aline Torrezin</b></p>
<p>a.torrezin@gmail.com</p>
<p><b>Evandro Junior</b></p>
<p>evandro.1760@gmail.com</p>
</body>
</html>
