/*
** pamela.h for Project in /home/enguerrand/delivery/Project/pamela.h
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  Mon Oct 23 11:26:42 2017 Enguerrand Allamel
** Last update Mon Oct 23 11:26:42 2017 Enguerrand Allamel
*/

#ifndef PROJECT_PAMELA_H
#define PROJECT_PAMELA_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAM_SM_PASSWORD
#define PAM_SM_SESSION
#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <libcryptsetup.h>

#include <pwd.h>

#define BUFFER_SIZE     512
#define CONTAINER_SIZE  1024

int		open_container(const char*, const char*);
int		close_container(const char*);
int		change_container_mdp(const char*, const char*, const char*);

int		secure_volume_open(const char *user, const char *pass);
int		secure_volume_change_mdp(const char *user, const char *pass_old, const char *pass_new);
int 	secure_volume_format(const char *user, const char *pass);

const char 		*get_user_home(const char *user);

#endif //PROJECT_PAMELA_H
