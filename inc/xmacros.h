/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmacros.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:37:32 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 11:38:31 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XMACROS_H
# define XMACROS_H

// Keys
# define KEY_Q				'q'
# define KEY_E				'e'
# define KEY_W				'w'
# define KEY_A				'a'
# define KEY_S				's'
# define KEY_D				'd'
# define KEY_R				'r'
# define KEY_F				'f'
# define KEY_ONE			'1'
# define KEY_TWO			'2'
# define KEY_THREE			'3'
# define KEY_FOUR			'4'
# define KEY_ESC			65307
# define KEY_UP				65362
# define KEY_DOWN			65364
# define KEY_LEFT			65361
# define KEY_RIGHT			65363
# define KEY_LCTRL			65507
# define KEY_LSHIFT			65505

// Mouse buttons
# define BTN_LMB			1
# define BTN_MMB			2
# define BTN_RMB			3
# define BTN_MWU			4
# define BTN_MWD			5

// Event names
# define KEYPRESS			2
# define KEYRELEASE			3
# define BUTTONPRESS		4
# define BUTTONRELEASE		5
# define MOTIONNOTIFY		6
# define ENTERNOTIFY		7
# define LEAVENOTIFY		8
# define FOCUSIN			9
# define FOCUSOUT			10
# define KEYMAPNOTIFY		11
# define EXPOSE				12
# define GRAPHICSEXPOSE		13
# define NOEXPOSE			14
# define VISIBILITYNOTIFY	15
# define CREATENOTIFY		16
# define DESTROYNOTIFY		17
# define UNMAPNOTIFY		18
# define MAPNOTIFY			19
# define MAPREQUEST			20
# define REPARENTNOTIFY		21
# define CONFIGURENOTIFY	22
# define CONFIGUREREQUEST	23
# define GRAVITYNOTIFY		24
# define RESIZEREQUEST		25
# define CIRCULATENOTIFY	26
# define CIRCULATEREQUEST	27
# define PROPERTYNOTIFY		28
# define SELECTIONCLEAR		29
# define SELECTIONREQUEST	30
# define SELECTIONNOTIFY	31
# define COLORMAPNOTIFY		32
# define CLIENTMESSAGE		33
# define MAPPINGNOTIFY		34
# define GENERICEVENT		35
# define LASTEVENT			36

// Event masks
// Read these as (1L << 0), (1L << 1), etc...
// Only, norminette doesn't allow it...
# define NOEVENTMASK				0
# define KEYPRESSMASK				0b1
# define KEYRELEASEMASK				0b10
# define BUTTONPRESSMASK			0b100
# define BUTTONRELEASEMASK			0b1000
# define ENTERWINDOWMASK			0b10000
# define LEAVEWINDOWMASK			0b100000
# define POINTERMOTIONMASK			0b1000000
# define POINTERMOTIONHINTMASK		0b10000000
# define BUTTON1MOTIONMASK			0b100000000
# define BUTTON2MOTIONMASK			0b1000000000
# define BUTTON3MOTIONMASK			0b10000000000
# define BUTTON4MOTIONMASK			0b100000000000
# define BUTTON5MOTIONMASK			0b1000000000000
# define BUTTONMOTIONMASK			0b10000000000000
# define KEYMAPSTATEMASK			0b100000000000000
# define EXPOSUREMASK				0b1000000000000000
# define VISIBILITYCHANGEMASK		0b10000000000000000
# define STRUCTURENOTIFYMASK		0b100000000000000000
# define RESIZEREDIRECTMASK			0b1000000000000000000
# define SUBSTRUCTURENOTIFYMASK		0b10000000000000000000
# define SUBSTRUCTUREREDIRECTMASK	0b100000000000000000000
# define FOCUSCHANGEMASK			0b1000000000000000000000
# define PROPERTYCHANGEMASK			0b10000000000000000000000
# define COLORMAPCHANGEMASK			0b100000000000000000000000
# define OWNERGRABBUTTONMASK		0b1000000000000000000000000

#endif
