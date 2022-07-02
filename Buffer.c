/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
***********************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Modifier: Megan Machkouri
* Course: CST 8152 – Compilers, Lab Section: 013
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer (A12)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

/*
***********************************************************
* Function name: bCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: Ver 2.1
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
* TO_DO: 
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer bCreate(carthage_int size, carthage_int increment, carthage_int mode) {
	BufferPointer b;
	/* TO_DO: Defensive programming */
	if (size<0 || size>CARTHAGE_MAX_SIZE)
		return NULL;
	if (!size) {
		size = BUFFER_DEFAULT_SIZE;
		increment = BUFFER_DEFAULT_INCREMENT;
	}
	if (!increment)
		mode = MODE_FIXED;
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	/* TO_DO: Defensive programming */
	if (b == NULL)
		return NULL;
	b->string = (carthage_chr*)malloc(size);
	/* TO_DO: Defensive programming */
	if (!b->string) {
		free(b);
		return NULL;
	}
	b->mode = mode;
	b->size = size;
	b->increment = increment;
	/* TO_DO: Initialize flags */
	b->flags = CARTHAGE_DEFAULT_FLAG;
	/* TO_DO: The created flag must be signalized as EMP */
	b->flags = b->flags | CARTHAGE_SET_FLAG_EMP;
	return b;
}


/*
***********************************************************
* Function name: bAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value:
*	bPointer (pointer to Buffer)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer bAddChar(BufferPointer const pBuffer, carthage_chr ch) {
	carthage_chr* tempbuf;
	carthage_int newSize;
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NULL;
	/* TO_DO: Reset Realocation */
	pBuffer->flags = pBuffer->flags & CARTHAGE_RST_FLAG_RLB;
	/* TO_DO: Test the inclusion of chars */
	if (pBuffer->position.writePos * (carthage_int)sizeof(carthage_chr) < pBuffer->size) {
		/* TO_DO: This buffer is NOT full */
		pBuffer->flags = pBuffer->flags & CARTHAGE_RST_FLAG_FUL;
	} else {
		/* TO_DO: Reset Full flag */
		pBuffer->flags = pBuffer->flags | CARTHAGE_SET_FLAG_FUL;
		switch (pBuffer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = pBuffer->size + pBuffer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < 0 || newSize <= pBuffer->size) {
				return NULL;
			}
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		case MODE_MULTI:
			newSize = pBuffer->size * pBuffer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < 0 || newSize <= pBuffer->size) {
				return NULL;
			}
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		default:
			return NULL;
		}
		tempbuf = (carthage_chr*)realloc(pBuffer->string, newSize);
		/* TO_DO: Defensive programming */
		if (!tempbuf)
			return NULL;
		if (tempbuf != pBuffer->string) {
			/* TO_DO: Set Relocation */
			pBuffer->flags = pBuffer->flags | CARTHAGE_SET_FLAG_RLB;
			pBuffer->string = tempbuf;
		}
		pBuffer->size = newSize;
	}
	/* TO_DO: Add the char */
	pBuffer->flags = pBuffer->flags & CARTHAGE_RST_FLAG_EMP;
	pBuffer->string[pBuffer->position.writePos++] = ch;
	return pBuffer;
}

/*
***********************************************************
* Function name: bClear
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bClear(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return CARTHAGE_FALSE;
	pBuffer->position.writePos = pBuffer->position.markPos = pBuffer->position.readPos = 0;
	/* TO_DO: Adjust flags original */
	pBuffer->flags = CARTHAGE_DEFAULT_FLAG;
	return CARTHAGE_TRUE;
}

/*
***********************************************************
* Function name: bDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bDestroy(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return CARTHAGE_FALSE;
	free(pBuffer->string);
	free(pBuffer);
	return CARTHAGE_TRUE;
}

/*
***********************************************************
* Function name: bIsFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bIsFull(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return CARTHAGE_FALSE;
	/* TO_DO: Check flag if buffer is FUL */
	if (pBuffer->flags & CARTHAGE_CHK_FLAG_FUL)
		return CARTHAGE_TRUE;
	return CARTHAGE_FALSE;
}

/*
***********************************************************
* Function name: bGetWritePos
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bGetWritePos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.writePos;
}

/*
***********************************************************
* Function name: bGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bGetSize(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->size;
}

/*
***********************************************************
* Function name: bGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bGetMode(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return (carthage_chr)BUFFER_ERROR;
	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bGetMarkPos
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bGetMarkPos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.markPos;
}


/*
***********************************************************
* Function name: bSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bSetMark(BufferPointer const pBuffer, carthage_int mark) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || mark<0 || mark > pBuffer->position.writePos)
		return CARTHAGE_FALSE;
	pBuffer->position.markPos = mark;
	return CARTHAGE_TRUE;
}

/*
***********************************************************
* Function name: bPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bPrint(BufferPointer const pBuffer) {
	carthage_int cont = 0;
	carthage_chr c;
	/* TO_DO: Defensive programming */
	if (!pBuffer || !(pBuffer->string))
		return BUFFER_ERROR;
	c = bGetChar(pBuffer);
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (!(pBuffer->flags & CARTHAGE_CHK_FLAG_EOB)) {
		cont++;
		printf("%c", c);
		c = bGetChar(pBuffer);
	}
	return cont;
}

/*
***********************************************************
* Function name: bLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bLoad(BufferPointer const pBuffer, FILE* const fi) {
	carthage_int size = 0;
	carthage_chr c;
	/* TO_DO: Defensive programming */
	if (!fi || !pBuffer)
		return BUFFER_ERROR;
	c = (carthage_chr)fgetc(fi);
	while (!feof(fi)) {
		if (!bAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (char)fgetc(fi);
		size++;
	}
	if (ferror(fi))
		return BUFFER_ERROR;
	return size;
}

/*
***********************************************************
* Function name: bIsEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bIsEmpty(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return CARTHAGE_FALSE;
	/* TO_DO: Check flag if buffer is EMP */
	if (pBuffer->flags & CARTHAGE_CHK_FLAG_EMP)
		return CARTHAGE_TRUE;
	return CARTHAGE_FALSE;
}

/*
***********************************************************
* Function name: bGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_chr bGetChar(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	if (pBuffer->position.readPos == pBuffer->position.writePos) {
		/* TO_DO: Set EOB flag */
		pBuffer->flags = pBuffer->flags | CARTHAGE_SET_FLAG_EOB;
		return BUFFER_EOF;
	}
	/* TO_DO: Reset EOB flag */
	pBuffer->flags = pBuffer->flags & CARTHAGE_RST_FLAG_EOB;
	return pBuffer->string[pBuffer->position.readPos++];
}


/*
***********************************************************
* Function name: bRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bRecover(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return CARTHAGE_FALSE;
	pBuffer->position.readPos = 0;
	pBuffer->position.markPos = 0;
	return CARTHAGE_TRUE;
}


/*
***********************************************************
* Function name: bRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bRetract(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || pBuffer->position.readPos == 0)
		return CARTHAGE_FALSE;
	pBuffer->position.readPos--;
	return CARTHAGE_TRUE;
}


/*
***********************************************************
* Function name: bRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_bol bRestore(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return CARTHAGE_FALSE;
	pBuffer->position.readPos = pBuffer->position.markPos;
	return CARTHAGE_TRUE;
}


/*
***********************************************************
* Function name: bGetReadPos
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bGetReadPos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.readPos;
}


/*
***********************************************************
* Function name: bGetIncrement
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_int bGetIncrement(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->increment;
}


/*
***********************************************************
* Function name: bGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
carthage_chr* bGetContent(BufferPointer const pBuffer, carthage_int pos) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || pos < 0 || pos > pBuffer->position.writePos)
		return NULL;
	return pBuffer->string + pos;
}


/*
***********************************************************
* Function name: bGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
carthage_flg bGetFlags(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return (carthage_flg)BUFFER_ERROR;
	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
