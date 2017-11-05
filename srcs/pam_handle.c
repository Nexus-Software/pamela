/*
** pam_handle.c for Project in /home/enguerrand/delivery/Project/pam_handle.c
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  Mon Oct 23 11:26:21 2017 Enguerrand Allamel
** Last update Mon Oct 23 11:26:21 2017 Enguerrand Allamel
*/

#include "pamela.h"

/**
 * PAM Clean up for pam data
 * @param pamh
 * @param data
 * @param error_status
 */
void  pam_me_cleanup(pam_handle_t 	*pamh,
					 void 			*data,
					 int 			error_status)
{
  if (data) {
	memset(data, 0, strlen(data));
	free(data);
  }
}

/**
 * PAM Account
 * @param pamh
 * @param flags
 * @param argc
 * @param argv
 * @return
 */
PAM_EXTERN int pam_sm_acct_mgmt( pam_handle_t 	*pamh,
								 int 			flags,
								 int 			argc,
								 const char 	**argv)
{
  return PAM_SUCCESS;
}

/**
 * PAM Change password
 * @param pamh
 * @param flags
 * @param argc
 * @param argv
 * @return
 */
PAM_EXTERN int pam_sm_setcred( pam_handle_t 	*pamh,
							   int 				flags,
							   int 				argc,
							   const char 		**argv)
{
  return PAM_SUCCESS;
}

/**
 * PAM Login
 * @param pamh
 * @param flags
 * @param argc
 * @param argv
 * @return
 */
PAM_EXTERN int 	pam_sm_authenticate(pam_handle_t	*pamh,
									  int 			flags,
									  int 			argc,
									  const char 	**argv)
{
  int			ret;
  const char	*pUsername;
  const char	*pPass;

  if ((ret = pam_get_user(pamh, &pUsername, "Username: ")) != PAM_SUCCESS)
	return (ret);
  else if ((ret = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&pPass)) != PAM_SUCCESS)
	return (ret);
  else if ((ret = pam_set_data(pamh, "pam_user_pass", strdup(pPass), &pam_me_cleanup)) != PAM_SUCCESS)
	return (ret);
  printf("[PAMela]: Get your auth key\n");
  return PAM_SUCCESS;
}

/**
 * PAM Open session
 * @param pamh
 * @param flags
 * @param argc
 * @param argv
 * @return
 */
PAM_EXTERN int 	pam_sm_open_session(pam_handle_t	*pamh,
									  int 			flags,
									  int 			argc,
									  const char 	**argv)
{
  int			ret;
  const char	*pUsername;
  const char	*pPass;

  printf("[PAMela]: Safe container, wait please ...\n");
  if ((ret = pam_get_user(pamh, &pUsername, "Username: ")) != PAM_SUCCESS)
	return (ret);
  else if ((ret = pam_get_data(pamh, "pam_user_pass", (const void **)&pPass)) != PAM_SUCCESS)
	return (ret);
  else if (open_container(pUsername, pPass) != 0)
	return PAM_SESSION_ERR;
  printf("[PAMela]: Safe container is in your home\n");
  return PAM_SUCCESS;
}

/**
 * PAM Close session
 * @param pamh
 * @param flags
 * @param argc
 * @param argv
 * @return
 */
PAM_EXTERN int 	pam_sm_close_session(pam_handle_t	*pamh,
									   int 			flags,
									   int 			argc,
									   const char 		**argv)
{
  int			ret;
  const char	*pUsername;

  if ((ret =  pam_get_user(pamh, &pUsername, "Username: ")) != PAM_SUCCESS)
	return (ret);
  else if (close_container(pUsername) != 0)
	return PAM_SESSION_ERR;
  printf("[PAMela]: Container is encrypt. Good bye\n");
  return PAM_SUCCESS;
}

/**
 * PAM Auth tok change
 * @param pamh
 * @param flags
 * @param argc
 * @param argv
 * @return
 */
PAM_EXTERN int 	pam_sm_chauthtok(pam_handle_t	*pamh,
								   int 			flags,
								   int 			argc,
								   const char 		**argv)
{
  int			ret;
  const char 	*pUsername;
  const char 	*pPass;
  const char 	*pPassOld;

  if ((ret = pam_get_user(pamh, &pUsername, "Username: ")) != PAM_SUCCESS)
	return (ret);
  else if ((ret = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&pPass)) != PAM_SUCCESS)
	return (ret);
  if (pPass) {
	printf("[PAMela]: Container change key ...\n");
	if ((ret = pam_get_item(pamh, PAM_OLDAUTHTOK, (const void **)&pPassOld)) != PAM_SUCCESS)
	  return (ret);
	else if (change_container_mdp(pUsername, pPassOld, pPass) != 0)
	  return PAM_SYSTEM_ERR;
	printf("[PAMela]: Container change key DONE\n");
  }
  return PAM_SUCCESS;
}