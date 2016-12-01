/*
*	Grupo 12
* @author Daniel Santos 44887
* @author Luis Barros  47082
* @author Marcus Dias 44901
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "../include/client_stub-private.h"
#include "../include/codes.h"
#include "../include/message-private.h"
#include "../include/network_client-private.h"
#include "../include/table.h"

/**
* Devolve o ip primário
*/
char *get_prim(struct rtable_t *table){
	if(table->primario == IP_ADDR_1)
		return table->ip_addr1;
	else
		return table->ip_addr2;
}

/**
* Atualiza o atributo de primario para o novo server primario
*/
void switch_pserver(struct rtable_t *table){
	if(table->primario == IP_ADDR_1)
		table->primario = IP_ADDR_2;
	else
		table->primario = IP_ADDR_1;
}

struct message_t *network_reconnect(struct rtable_t *table, struct message_t *msg_pedido, struct message_t *msg_resposta){
	
	table->server = network_connect(get_prim(table));
	printf("antes do if\n");
	if(table->server != NULL)
		printf("server not null\n");
	else
		printf("server null\n");
		
	if(table->server != NULL){
		// Segunda tentativa a contactar o servidor primário
		// Se falhar fazer bind ao secundário
		// e alterar quem é o primário e secundário
		msg_resposta = network_send_receive(table->server, msg_pedido);
		if(msg_resposta == NULL){
			perror("Sem resposta do servidor primário\n");
			printf("Estabelecer conexão ao secundário\n");					
			// Alterar idenficador de quem é o primário 1 -> 2
			switch_pserver(table);
			// Fecha a ligação
			/*if(network_close(table->server) < 0){
				perror("Problema ao terminar a associação entre cliente e tabela remota\n");
			}
			// Fazer bind ao servidor secundário
			// Se bind falhar, tentar novamente
			table = rtable_bind(get_prim(table));
			if(table == NULL){
				sleep(RETRY_TIME);
				// Se 2º bind falhar parar
				// Fecha a ligação
				table = rtable_bind(get_prim(table));
				if(table == NULL){return NULL;}
			}*/
			// Conectar ao secundário se falhar esperar e conectar outra vez
			// Se correr bem enviar msg					
			table->server = network_connect(get_prim(table));
			if(table->server != NULL){
				printf("Conexão estabecida com secundário\n");
				// Enviar mensagem ao servidor se falhar tentar outra vez
				msg_resposta = network_send_receive(table->server, msg_pedido);
				if(msg_resposta == NULL){
					sleep(RETRY_TIME);
					// Última tentativa
					msg_resposta = network_send_receive(table->server, msg_pedido);
					if(msg_resposta == NULL){
						perror("Problema a conectar ao servidor\n");
						printf("Não existem servidores disponíveis\n");
						return NULL;
					}
				}
			}else{
				sleep(RETRY_TIME);
				table->server = network_connect(get_prim(table));
				if(table->server == NULL){
					perror("Problema a conectar ao secundário\n");
					printf("Não existem servidores disponíveis\n");
					return NULL;	
				}else{
					printf("Conexão estabecida com secundário\n");
					// Enviar mensagem ao servidor se falhar tentar outra vez
					msg_resposta = network_send_receive(table->server, msg_pedido);
					if(msg_resposta == NULL){
						sleep(RETRY_TIME);
						// Última tentativa
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							perror("Problema a conectar ao servidor\n");
							printf("Não existem servidores disponíveis\n");
							return NULL;
						}
					}						
					
				}
			}			
		}	
	}else{ // table->server == NULL
		perror("Sem resposta do servidor primário\n");
		printf("Estabelecer conexão ao secundário\n");				
		// Alterar idenficador de quem é o primário 1 -> 2
		printf("table->primario %i\n", table->primario);
		printf("table->primario %s\n", get_prim(table));
		switch_pserver(table);
		printf("table->primario %i\n", table->primario);
		printf("table->primario %s\n", get_prim(table));
		// Fazer bind ao servidor secundário
		// Se bind falhar, tentar novamente
		// Fecha a ligação
		//if(network_close(table->server) < 0){
		//	perror("Problema ao terminar a associação entre cliente e tabela remota\n");
		//}
		/*table = rtable_bind(get_prim(table));
		//if(table != NULL){printf("após bind\n");}
		if(table == NULL){
			printf("table == null\n");
			sleep(RETRY_TIME);
			// Se 2º bind falhar parar
			// Fecha a ligação
			if(network_close(table->server) < 0){
				perror("Problema ao terminar a associação entre cliente e tabela remota\n");
			}		
			table = rtable_bind(get_prim(table));
			if(table == NULL){return NULL;}
		}*/
		printf("primario %s\n", get_prim(table));
		printf("fez bind ao 2\n");
		// Conectar ao secundário se falhar esperar e conectar outra vez
		// Se correr bem enviar msg			
		table->server = network_connect(get_prim(table));
		printf("connect com o 2\n");
		if(table->server != NULL){
			printf("server not null\n");
			printf("Conexão estabecida com secundário\n");
			// Enviar mensagem ao servidor se falhar tentar outra vez*/
			msg_resposta = network_send_receive(table->server, msg_pedido);
			if(msg_resposta == NULL){
				sleep(RETRY_TIME);
				// Última tentativa
				msg_resposta = network_send_receive(table->server, msg_pedido);
				if(msg_resposta == NULL){
					perror("Problema a conectar ao servidor\n");
					printf("Não existem servidores disponíveis\n");
					return NULL;
				}
			}
		}else{
			printf("server null\n");
			sleep(RETRY_TIME);
			table->server = network_connect(get_prim(table));
			if(table->server == NULL){
				perror("Problema a conectar ao secundário\n");
				printf("Não existem servidores disponíveis\n");
				return NULL;	
			}else{
				printf("Conexão estabecida com secundário\n");
				// Enviar mensagem ao servidor se falhar tentar outra vez
				msg_resposta = network_send_receive(table->server, msg_pedido);
				if(msg_resposta == NULL){
					sleep(RETRY_TIME);
					// Última tentativa
					msg_resposta = network_send_receive(table->server, msg_pedido);
					if(msg_resposta == NULL){
						perror("Problema a conectar ao servidor\n");
						printf("Não existem servidores disponíveis\n");
						return NULL;
					}
				}						
			}			
		}
	}
	return msg_resposta;
}

/**
*	:::: faz o pendido e se der erro tenta novamente depois do tempo de retry :::
*
*/
struct message_t *network_with_retry(struct rtable_t *table, struct message_t *msg_pedido){
	struct message_t *msg_resposta;
	// Primeira tentativa a contactar servidor primário
	msg_resposta = network_send_receive(table->server, msg_pedido);

	if(msg_resposta == NULL){
		// Não consegu contactar
		// Aguarda um tempo e tenta de novo
		perror("Problema com a mensagem de resposta, tentar novamente..\n");
		sleep(RETRY_TIME);

		// Fecha a ligação
		if(network_close(table->server) < 0){
			perror("Problema ao terminar a associação entre cliente e tabela remota\n");
		}
		
		msg_resposta = network_reconnect(table, msg_pedido, msg_resposta);
		if(msg_resposta == NULL){
			printf("msg_resposta no network_with retry está a null\n");
		}
		// Nova ligação
		/*// Tentar conectar novamente ao primário
		table->server = network_connect(table->ip_addr1);
		if(table->server == NULL){
			perror("Problema na conecção\n");
			return NULL;
		}
		// Segunda tentativa a contactar o servidor
		msg_resposta = network_send_receive(table->server, msg_pedido);
		if(msg_resposta == NULL){
			// Não consegue e desiste
			// Deverá ser aqui que contacta o servidor secundario
			perror("sem resposta do servidor\n");
		}
		// Fim do código de conectar ao primário*/
		
		// Conectar ao primário
		/**if(table->primario == IP_ADDR_1){
			printf("primário %i\n", table->primario);
			printf("ip1 %s\n", table->ip_addr1);
			printf("ip2 %s\n", table->ip_addr2);	
			table->server = network_connect(table->ip_addr1);
			if(table->server != NULL){
				// Segunda tentativa a contactar o servidor primário
				// Se falhar fazer bind ao secundário
				// e alterar quem é o primário e secundário
				msg_resposta = network_send_receive(table->server, msg_pedido);
				if(msg_resposta == NULL){
					perror("Sem resposta do servidor primário\n");
					printf("Estabelecer conexão ao secundário\n");					
					// Alterar idenficador de quem é o primário 1 -> 2
					table->primario = IP_ADDR_2;
					// Fecha a ligação
					if(network_close(table->server) < 0){
						perror("Problema ao terminar a associação entre cliente e tabela remota\n");
					}
					// Fazer bind ao servidor secundário
					// Se bind falhar, tentar novamente
					table = rtable_bind(table->ip_addr2);
					if(table == NULL){
						sleep(RETRY_TIME);
						// Se 2º bind falhar parar
						// Fecha a ligação
						if(network_close(table->server) < 0){
							perror("Problema ao terminar a associação entre cliente e tabela remota\n");
						}
						table = rtable_bind(table->ip_addr2);
						if(table == NULL){return NULL;}
					}
					// Conectar ao secundário se falhar esperar e conectar outra vez
					// Se correr bem enviar msg					
					table->server = network_connect(table->ip_addr2);
					if(table->server != NULL){
						printf("Conexão estabecida com secundário\n");
						// Enviar mensagem ao servidor se falhar tentar outra vez
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							sleep(RETRY_TIME);
							// Última tentativa
							msg_resposta = network_send_receive(table->server, msg_pedido);
							if(msg_resposta == NULL){
								perror("Problema a conectar ao servidor\n");
								printf("Não existem servidores disponíveis\n");
								return NULL;
							}
						}
					}else{
						sleep(RETRY_TIME);
						table->server = network_connect(table->ip_addr2);
						if(table->server == NULL){
							perror("Problema a conectar ao secundário\n");
							printf("Não existem servidores disponíveis\n");
							return NULL;	
						}else{
							printf("Conexão estabecida com secundário\n");
							// Enviar mensagem ao servidor se falhar tentar outra vez
							msg_resposta = network_send_receive(table->server, msg_pedido);
							if(msg_resposta == NULL){
								sleep(RETRY_TIME);
								// Última tentativa
								msg_resposta = network_send_receive(table->server, msg_pedido);
								if(msg_resposta == NULL){
									perror("Problema a conectar ao servidor\n");
									printf("Não existem servidores disponíveis\n");
									return NULL;
								}
							}						
							
						}
					}			
				}	
			}else{ // table->server == NULL
				perror("Sem resposta do servidor primário\n");
				printf("Estabelecer conexão ao secundário\n");
				//sleep(RETRY_TIME);
				if(network_close(table->server) < 0){
					perror("Problema ao terminar a associação entre cliente e tabela remota\n");
				}					
				// Alterar idenficador de quem é o primário 1 -> 2
				table->primario = IP_ADDR_2;
				// Fazer bind ao servidor secundário
				// Se bind falhar, tentar novamente
				printf("ip_addr2 %s\n", table->ip_addr2);
				// Fecha a ligação
				if(network_close(table->server) < 0){
					perror("Problema ao terminar a associação entre cliente e tabela remota\n");
				}
				table = rtable_bind(table->ip_addr2);
				if(table == NULL){
					sleep(RETRY_TIME);
					// Se 2º bind falhar parar
					// Fecha a ligação
					if(network_close(table->server) < 0){
						perror("Problema ao terminar a associação entre cliente e tabela remota\n");
					}		
					table = rtable_bind(table->ip_addr2);
					if(table == NULL){return NULL;}
				}
				printf("fez bind ao 2\n");
				// Conectar ao secundário se falhar esperar e conectar outra vez
				// Se correr bem enviar msg					
				table->server = network_connect(table->ip_addr2);
				printf("connect com o 2\n");
				if(table->server != NULL){
					printf("server not null\n");
					printf("Conexão estabecida com secundário\n");
					// Enviar mensagem ao servidor se falhar tentar outra vez
					msg_resposta = network_send_receive(table->server, msg_pedido);
					if(msg_resposta == NULL){
						sleep(RETRY_TIME);
						// Última tentativa
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							perror("Problema a conectar ao servidor\n");
							printf("Não existem servidores disponíveis\n");
							return NULL;
						}
					}
				}else{
					printf("server null\n");
					sleep(RETRY_TIME);
					table->server = network_connect(table->ip_addr2);
					if(table->server == NULL){
						perror("Problema a conectar ao secundário\n");
						printf("Não existem servidores disponíveis\n");
						return NULL;	
					}else{
						printf("Conexão estabecida com secundário\n");
						// Enviar mensagem ao servidor se falhar tentar outra vez
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							sleep(RETRY_TIME);
							// Última tentativa
							msg_resposta = network_send_receive(table->server, msg_pedido);
							if(msg_resposta == NULL){
								perror("Problema a conectar ao servidor\n");
								printf("Não existem servidores disponíveis\n");
								return NULL;
							}
						}						
					}			
				}
			}
		// Primário é o IP_ADDR_2
		}else if(table->primario == IP_ADDR_2){
			table->server = network_connect(table->ip_addr2);
			if(table->server != NULL){
				// Segunda tentativa a contactar o servidor primário
				// Se falhar fazer bind ao secundário
				// e alterar quem é o primário e secundário
				msg_resposta = network_send_receive(table->server, msg_pedido);
				if(msg_resposta == NULL){
					perror("Sem resposta do servidor primário\n");
					printf("Estabelecer conexão ao secundário\n");
					// Alterar idenficador de quem é o primário 2 -> 1
					table->primario = IP_ADDR_1;
					// Fazer bind ao servidor secundário
					// Se bind falhar, tentar novamente
					// Fecha a ligação
					if(network_close(table->server) < 0){
						perror("Problema ao terminar a associação entre cliente e tabela remota\n");
					}
					table = rtable_bind(table->ip_addr1);
					if(table == NULL){
						sleep(RETRY_TIME);
						// Se 2º bind falhar parar
						table = rtable_bind(table->ip_addr1);
						if(table == NULL){return NULL;}
					}
					// Conectar ao secundário se falhar esperar e conectar outra vez
					// Se correr bem enviar msg
					table->server = network_connect(table->ip_addr1);
					if(table->server != NULL){
						printf("Conexão estabecida com secundário\n");
						// Enviar mensagem ao servidor se falhar tentar outra vez
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							sleep(RETRY_TIME);
							// Última tentativa
							msg_resposta = network_send_receive(table->server, msg_pedido);
							if(msg_resposta == NULL){
								perror("Problema a conectar ao servidor\n");
								printf("Não existem servidores disponíveis\n");
								return NULL;
							}
						}
					}else{
						sleep(RETRY_TIME);
						table->server = network_connect(table->ip_addr1);
						if(table->server == NULL){
							perror("Problema a conectar ao secundário\n");
							printf("Não existem servidores disponíveis\n");
							return NULL;	
						}else{
							printf("Conexão estabecida com secundário\n");
							// Enviar mensagem ao servidor se falhar tentar outra vez
							msg_resposta = network_send_receive(table->server, msg_pedido);
							if(msg_resposta == NULL){
								sleep(RETRY_TIME);
								// Última tentativa
								msg_resposta = network_send_receive(table->server, msg_pedido);
								if(msg_resposta == NULL){
									perror("Problema a conectar ao servidor\n");
									printf("Não existem servidores disponíveis\n");
									return NULL;
								}
							}						
						
						}
					}			
				}
			}else{ // table->server == NULL
				perror("Sem resposta do servidor primário\n");
				printf("Estabelecer conexão ao secundário\n");					
				// Alterar idenficador de quem é o primário 1 -> 2
				table->primario = IP_ADDR_2;
				// Fazer bind ao servidor secundário
				// Se bind falhar, tentar novamente
				// Fecha a ligação
				if(network_close(table->server) < 0){
					perror("Problema ao terminar a associação entre cliente e tabela remota\n");
				}
				table = rtable_bind(table->ip_addr2);
				if(table == NULL){
					sleep(RETRY_TIME);
					// Se 2º bind falhar parar
					table = rtable_bind(table->ip_addr2);
					if(table == NULL){return NULL;}
				}
				// Conectar ao secundário se falhar esperar e conectar outra vez
				// Se correr bem enviar msg					
				table->server = network_connect(table->ip_addr2);
				if(table->server != NULL){
					printf("Conexão estabecida com secundário\n");
					// Enviar mensagem ao servidor se falhar tentar outra vez
					msg_resposta = network_send_receive(table->server, msg_pedido);
					if(msg_resposta == NULL){
						sleep(RETRY_TIME);
						// Última tentativa
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							perror("Problema a conectar ao servidor\n");
							printf("Não existem servidores disponíveis\n");
							return NULL;
						}
					}
				}else{
					sleep(RETRY_TIME);
					table->server = network_connect(table->ip_addr2);
					if(table->server == NULL){
						perror("Problema a conectar ao secundário\n");
						printf("Não existem servidores disponíveis\n");
						return NULL;	
					}else{
						printf("Conexão estabecida com secundário\n");
						// Enviar mensagem ao servidor se falhar tentar outra vez
						msg_resposta = network_send_receive(table->server, msg_pedido);
						if(msg_resposta == NULL){
							sleep(RETRY_TIME);
							// Última tentativa
							msg_resposta = network_send_receive(table->server, msg_pedido);
							if(msg_resposta == NULL){
								perror("Problema a conectar ao servidor\n");
								printf("Não existem servidores disponíveis\n");
								return NULL;
							}
						}						
					}			
				}
			}
		}// Verificar quem é o primário*/
	}
	//retorna resposta seja null ou nao
	return msg_resposta;

}



/* Função para estabelecer uma associação entre o cliente e uma tabela
 * remota num servidor.
 * address_port é uma string no formato <hostname>:<port>.
 * retorna NULL em caso de erro .
 */
struct rtable_t *rtable_bind(const char *address_port){

	if(address_port == NULL){
		perror("Problema com o address_port\n");
		return NULL;
	}

	struct rtable_t *rtable = (struct rtable_t *)malloc(sizeof(struct rtable_t));
	if(rtable == NULL){
		perror("Problema na criação da tabela remota\n");
		return NULL;
	}	
	
	rtable->server = network_connect(address_port);
	if(rtable->server == NULL){
		perror("Problema na conecção\n");
		return NULL;
	}
	return rtable;	
}

/* Termina a associação entre o cliente e a tabela remota, e liberta
 * toda a memória local. 
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtable_unbind(struct rtable_t *rtable){
	if(network_close(rtable->server) < 0){
		perror("Problema ao terminar a associação entre cliente e tabela remota\n");
		// Liberta a memoria
		free(rtable->ip_addr1);
		free(rtable->ip_addr2);
		free(rtable);

		return ERROR;
	}
	// Memoria da estrutura server_t ja libertada ao fazer network_close
	// Libertar o resto da rtable
	//free(rtable->primario);
	//free(rtable->secundario);
	free(rtable);
	return OK;
}

/* Função para adicionar um par chave valor na tabela remota.
 * Devolve 0 (ok) ou -1 (problemas).
 */
int rtable_put(struct rtable_t *rtable, char *key, struct data_t *value){
	struct message_t *msg_resposta, *msg_pedido;
	// Verificação se a rtable, key e value são válidos
	if(rtable == NULL || key == NULL || value == NULL){
		perror("Problema com rtable/key/value\n");
		return ERROR;
	}
	// Criação e alocação da mensagem de pedido
	msg_pedido = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg_pedido == NULL){
		perror("Problema na criação da mensagem de pedido\n");
		return ERROR;
	}
	// Mensagem a enviar
	msg_pedido->opcode = OC_PUT;
	msg_pedido->c_type = CT_ENTRY;
	msg_pedido->content.entry = entry_create(key, value);
	// Receber a mensagem de resposta
	msg_resposta = network_with_retry(rtable, msg_pedido);
	//msg_resposta = network_send_receive(rtable->server, msg_pedido);
	if(msg_resposta == NULL){
		//perror("Problema com a mensagem de resposta\n");
		return ERROR;
	}
	// Mensagem de pedido já não é necessária
	//free_message(msg_pedido);
	return msg_resposta->content.result;
}

/* Função para substituir na tabela remota, o valor associado à chave key.
 * Devolve 0 (OK) ou -1 em caso de erros.
 */
int rtable_update(struct rtable_t *rtable, char *key, struct data_t *value){
	struct message_t *msg_resposta, *msg_pedido;
	// Verificação se a rtable, key e value são válidos
	if(rtable == NULL || key == NULL || value == NULL){
		perror("Problema com rtable/key/value\n");
		return ERROR;
	}
	// Criação e alocação da mensagem de pedido
	msg_pedido = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg_pedido == NULL){
		perror("Problema na criação da mensagem de pedido\n");
		return ERROR;
	}
	// Mensagem a enviar
	msg_pedido->opcode = OC_UPDATE;
	msg_pedido->c_type = CT_ENTRY;
	msg_pedido->content.entry = entry_create(key, value);
	// Receber a mensagem de resposta
	msg_resposta = network_with_retry(rtable, msg_pedido);
	//msg_resposta = network_send_receive(rtable->server, msg_pedido);
	if(msg_resposta == NULL){
	//	perror("Problema com a mensagem de resposta\n");
		return ERROR;
	} 
	// Mensagem de pedido já não é necessária
	//free_message(msg_pedido);
	return msg_resposta->content.result;	
}

/* Função para obter da tabela remota o valor associado à chave key.
 * Devolve NULL em caso de erro.
 */
struct data_t *rtable_get(struct rtable_t *table, char *key){
	struct message_t *msg_resposta, *msg_pedido;
	// Verificação se a rtable e key são válidos
	if(table == NULL || key == NULL){
		perror("Problema com rtable/key\n");
		return NULL;
	}
	// Criação e alocação da mensagem de pedido
	msg_pedido = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg_pedido == NULL){
		perror("Problema na criação da mensagem de pedido\n");
		return NULL;
	}
	// Mensagem a enviar
	msg_pedido->opcode = OC_GET;
	msg_pedido->c_type = CT_KEY;
	msg_pedido->content.key = key;
	// Receber a mensagem de resposta
	msg_resposta = network_with_retry(table, msg_pedido);
	//msg_resposta = network_send_receive(table->server, msg_pedido);
	if(msg_resposta == NULL){
	//	perror("Problema com a mensagem de resposta\n");
		return NULL;
	}
	//Se data vem a NULL é porque não existem keys
	if(msg_resposta->content.data == NULL){
		char *noKeys = "essa chave não existe";
		return data_create2(strlen(noKeys), noKeys);
	}
	// Mensagem de pedido já não é necessária
	//free(msg_pedido);
	return msg_resposta->content.data;
}

/* Função para remover um par chave valor da tabela remota, especificado 
 * pela chave key.
 * Devolve: 0 (OK) ou -1 em caso de erros.
 */
int rtable_del(struct rtable_t *table, char *key){
	struct message_t *msg_resposta, *msg_pedido;
	// Verificação se a rtable e key são válidos
	if(table == NULL || key == NULL){
		perror("Problema com rtable/key\n");
		return ERROR;
	}
	// Criação e alocação da mensagem de pedido
	msg_pedido = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg_pedido == NULL){
		perror("Problema na criação da mensagem de pedido\n");
		return ERROR;
	}
	// Mensagem a enviar
	msg_pedido->opcode = OC_DEL;
	msg_pedido->c_type = CT_KEY;
	msg_pedido->content.key = key;
	// Receber a mensagem de resposta
	msg_resposta = network_with_retry(table, msg_pedido);
	//msg_resposta = network_send_receive(table->server, msg_pedido);
	if(msg_resposta == NULL){
	//	perror("Problema com a mensagem de resposta\n");
		return ERROR;
	} 
	// Mensagem de pedido já não é necessária
	//free_message(msg_pedido);
	return msg_resposta->content.result;	

}

/* Devolve número de pares chave/valor na tabela remota.
 */
int rtable_size(struct rtable_t *rtable){
	struct message_t *msg_resposta, *msg_pedido;
	// Verificar se rtable é válido
	if(rtable == NULL){
		perror("Problema com rtable\n");
		return ERROR;
	}
	// Criação e alocação da mensagem de pedido
	msg_pedido = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg_pedido == NULL){
		perror("Problema na criação da mensagem de pedido\n");
		return ERROR;
	}
	// Mensagem a enviar
	msg_pedido->opcode = OC_SIZE;
	msg_pedido->c_type = CT_RESULT;
	// Receber a mensagem de resposta
	msg_resposta = network_with_retry(rtable, msg_pedido);
	//msg_resposta = network_send_receive(rtable->server, msg_pedido);
	if(msg_resposta == NULL){
	//	perror("Problema com a mensagem de resposta\n");
		return ERROR;
	} 
	// Mensagem de pedido já não é necessária
	//free_message(msg_pedido);
	return msg_resposta->content.result;	
}

/* Devolve um array de char * com a cópia de todas as keys da
 * tabela remota, e um último elemento a NULL.
 */
char **rtable_get_keys(struct rtable_t *rtable){
	struct message_t *msg_resposta, *msg_pedido;
	char *all = "!";
	// Verificar se rtable é válido
	if(rtable == NULL){
		perror("Problema com rtable\n");
		return NULL;
	}
	// Criação e alocação da mensagem de pedido
	msg_pedido = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg_pedido == NULL){
		perror("Problema na criação da mensagem de pedido\n");
		return NULL;
	}
	// Mensagem a enviar
	msg_pedido->opcode = OC_GET;
	msg_pedido->c_type = CT_KEY;
	// Servidor vai verificar se content.key == !
	msg_pedido->content.key = all;
	// Receber a mensagem de resposta
	msg_resposta = network_with_retry(rtable, msg_pedido);
	//msg_resposta = network_send_receive(rtable->server, msg_pedido);
	if(msg_resposta == NULL){
	//	perror("Problema com a mensagem de resposta\n");
		return NULL;
	} 
	// Mensagem de pedido já não é necessária
	/////////////////////////////////////
	// Este free message dá problemas... Tentar perceber porquê
	/////////////////////////////////////
	//free_message(msg_pedido);
	return msg_resposta->content.keys;	
}

/* Liberta a memória alocada por table_get_keys().
 */
void rtable_free_keys(char **keys){
	table_free_keys(keys);
}

/*
Função que inicializa uma tabela remota
e aloca os endereços dos servidores
indentificando que servidor é o principal
*/
struct rtable_t *main_bind_rtable(const char *server1, const char *server2) {
	if(server1 == NULL || server2 == NULL){
		printf("problema com os endereços\n");
		return NULL;
	}		
	
	// Remote table
	struct rtable_t *rtable;
	// Inicializar remote table
	
	rtable = rtable_bind(server1);
	// Verifica criação
	if (rtable == NULL) {
		// Server1 indisponivel
		// Aguarda e tenta de novo
		printf("Tabela indisponivel... Tentar novamente\n");
		sleep(RETRY_TIME);
		rtable = rtable_bind(server1);

		if (rtable == NULL) {
			// Server1 indisponivel
			// Tenta server2
			rtable = rtable_bind(server2);
			if (rtable == NULL) {
				// Server 2 indisponivel
				// Aguarda e tenta de novo
				printf("Tabela indisponivel...Tentar novamente\n");
				sleep(RETRY_TIME);
				rtable = rtable_bind(server2);
				if (rtable == NULL) {
					// Impossivel de fazer bind
					// A qualquer um dos servidores
					// Não precisa de print que o table_client
					// Ao receber NULL envia mensagem de erro
					return NULL;

				} else {
					// Fez bind ao server 2
					rtable->primario = IP_ADDR_2;
				}

			} else {
				// Fez bind ao server2
				rtable->primario = IP_ADDR_2;
			}			

		} else {
			// Fez bind ao server1
			rtable->primario = IP_ADDR_1;
		}

		
	} else {
		// Fez bind ao server1
		rtable->primario = IP_ADDR_1;
	}
	
	// Retorna rtable
	rtable->ip_addr1 = strdup(server1);
	rtable->ip_addr2 = strdup(server2);
	return rtable;
}




