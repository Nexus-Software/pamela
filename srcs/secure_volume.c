/*
** secure_volume.c for Project in /home/enguerrand/delivery/Project/secure_volume.c
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  Fri Oct 27 17:15:12 2017 Enguerrand Allamel
** Last update Fri Oct 27 17:15:12 2017 Enguerrand Allamel
*/

#include "pamela.h"

/**
 * Open secure volume user
 * @param user username
 * @param pass password
 * @return 0 (success) / -1 (error)
 */
int						secure_volume_open(const char *user, const char *pass)
{
  struct crypt_device	*cryptdev;
  char  				volume_path[BUFFER_SIZE];
  char  				device_name[BUFFER_SIZE];

  if ((sprintf(volume_path, "%s/.crypted_container", get_user_home(user)) < 0) ||
	  (sprintf(device_name, "%s_container", user) < 0) ||
	  (crypt_init(&cryptdev, volume_path) < 0) ||
	  (crypt_load(cryptdev, CRYPT_LUKS1, NULL) < 0) ||
	  (crypt_activate_by_passphrase(cryptdev, device_name, CRYPT_ANY_SLOT, pass, strlen(pass), 0) < 0))
	return (-1);
  crypt_free(cryptdev);
  return (0);
}

/**
 * Change key for secure volume user
 * @param user username
 * @param pass_old old password
 * @param pass_new new password
 * @return 0 (success) / -1 (error)
 * @return
 */
int						secure_volume_change_mdp(const char *user, const char *pass_old, const char *pass_new)
{
  struct crypt_device	*cryptdev;
  char  				volume_path[BUFFER_SIZE];
  uid_t					uid = getuid();

  if ((setuid(0) != 0) ||
	  (sprintf(volume_path, "%s/.crypted_container", get_user_home(user)) < 0) ||
	  (crypt_init(&cryptdev, volume_path) < 0) ||
	  (crypt_load(cryptdev, CRYPT_LUKS1, NULL) < 0) ||
	  (crypt_keyslot_add_by_passphrase(cryptdev, CRYPT_ANY_SLOT, pass_old, strlen(pass_old), pass_new, strlen(pass_new)) < 0))
	return (-1);
  crypt_free(cryptdev);
  if (setuid(uid) != 0)
	return (-1);
  return (0);
}

/**
 * Format secure volume
 * @param user username
 * @param pass password
 * @return
 */
int 						secure_volume_format(const char *user, const char *pass)
{
  struct crypt_params_luks1 params;
  struct crypt_device		*cryptdev;
  char  					volume_path[BUFFER_SIZE];

  if ((sprintf(volume_path, "%s/.crypted_container", get_user_home(user)) < 0) ||
	  (crypt_init(&cryptdev, volume_path) < 0))
	return (-1);
  params.hash = "sha1";
  params.data_alignment = 0;
  params.data_device = NULL;
  if ((crypt_format(cryptdev, CRYPT_LUKS1, "aes", "xts-plain64", NULL, NULL, 256 / 8, &params) < 0) ||
	  (crypt_keyslot_add_by_volume_key(cryptdev, CRYPT_ANY_SLOT, NULL, 0, pass, strlen(pass)) < 0))
	return (-1);
  return (0);
}