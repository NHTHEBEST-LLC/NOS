/*
 * Copyright (c) 2006-2009 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Antoine Kaufmann.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>
#include <alloca.h>
#include <lost/config.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 0x7FFFFFFF

/** Maximale laenge eines Multibyte-Zeichens in Bytes */
#define MB_CUR_MAX 4

#ifdef __cplusplus
extern "C" {
#endif

void exit(int result);
void abort(void);
int atexit(void (*function)(void));

void* malloc(size_t size);
void free(void* address);
void* realloc(void* address, size_t size);
void* calloc(size_t size, size_t count);
void init_memory_manager(void);

long strtol(const char* str, char** endptr, int base);
long long strtoll(const char *str, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
unsigned long long strtoull(const char *nptr, char **endptr, int base);

long double strtold(const char* nptr, char** end);
double strtod(const char* nptr, char** end);
float strtof(const char* nptr, char** end);

int atoi(const char* s);
long atol(const char* str);

char* getenv(const char* name);
int setenv(const char* name, const char* value, int overwrite);

/**
 * Umgebungsvariable setzen
 *
 * @param str String in der Form variable=wert
 *
 * @return 0 bei Erfolg, -1 im Fehlerfall
 */
int putenv(const char* str);

void unsetenv(const char* name);

void qsort(void *base, size_t num, size_t size,
           int (*comparator)(const void *, const void *));

/**
 * Fuehrt eine Suche in einem sortierten Array von Objekten durch
 *
 * @param key Schluesselobjekt, das zum Vergleich mit dem gesuchten Objekt
 * verwendet wird
 * @param base Zeiger auf den Anfang des Arrays, das durchsucht wird
 * @param nel Anzahl der Elemente im Array
 * @param width Die Groesse eines Elements in bytes
 * @param compar Vergleichsfunktion, die angewandt wird, um die Suche
 * durchzufuehren. Jeder Kandidat wird durch diese Funktion mit dem
 * Schluesselobjekt verglichen. Wenn die Objekte gleich sind, gibt die
 * Vergleichsfunktion 0 zurueck und die Suche ist erfolgreich beendet.
 * Ansonsten gibt die Vergleichsfunktion < 0 zurueck, wenn der Schluessel
 * kleiner als der Kandidat ist; sie gibt > 0 zurueck, wenn die Schluessel
 * groesser ist
 *
 * @return Die Funktion gibt das gesuchte Objekt zurueck; wenn kein Objekt
 * gefunden wurde, gibt sie NULL zurueck.
 */
void *bsearch(const void *key, const void *base, size_t nel,
    size_t width, int (*compar)(const void *, const void *));

#ifndef CONFIG_LIBC_NO_STUBS
double atof(const char* str);
#endif

/**
 * Absolutbetrag einer Zahl errechnen
 */
static inline int abs(int x)
{
    return (x < 0 ? -x : x);
}

/**
 * Berechnet den Absolutwert eines long int
 */
static inline long int labs(long int x)
{
    return (x < 0 ? -x : x);
}

/**
 * Berechnet den Absolutwert eines long long int
 */
static inline long long int llabs(long long int x)
{
    return (x < 0 ? -x : x);
}

int system(const char* command);

/**
 * Anzahl der Bytes die das erste Zeichen belegt
 *
 * @param s     Pointer auf den Anfang den Anfang des Zeichens
 * @param slen  Maximale Laenge die das Zeichen haben kann (Stringlaenge)
 *
 * @return Laenge des Zeichens oder -1 wenn ein Fehler auftritt (z.B.
 *         ungueltiges Zeichen)
 */
int mblen(const char* s, size_t slen);

/**
 * Erstes Zeichen im String in einen wchar umwandeln. Wird NULL als wc
 * uebergeben, gibt die Funktion lediglich die Laenge des Zeichens zurueck
 * (mblen). Ist s NULL gibt die Funktion 0 zurueck.
 *
 * @param wc    Pointer auf den wchar in dem das Ergebnis abgelegt werden soll
 * @param s     Pointer auf den Anfang des Zeichens
 * @param len   Maximale Laenge die das Zeichen haben kann (Stringlaenge)
 *
 * @return Bei Erfolg wird die Anzahl der benutzten Bytes aus s zurueckgegeben,
 *         im Fehlerfall -1
 */
int mbtowc(wchar_t* wc, const char* s, size_t len);

/**
 * Einen wchar in ein Multibyte-Zeichen umwandeln. Der Aufrufer muss
 * garantieren dass in buf mindestens MB_CUR_MAX Bytes verfuegbar sind.
 *
 * Falls buf == NULL uebergeben wird, soll der interne "shift state"
 * zurueckgesetzt werden und != 0 zurueckgegeben werden, falls die aktuelle
 * Kodierung sowas hat. Solange wir nur UTF-8 benutzen brauchen wir uns aber
 * darum nicht zu kuemmern.
 *
 * @param buf Puffer in dem das Multibyte-Zeichen abgelegt wird. Dabei koennen
 *            bis zu MB_CUR_MAX Bytes geschrieben werden.
 * @param wc  Der umzuwandelnde wchar
 *
 * @return Bei Erfolg wir die Anzahl der in buf geschrieben Bytes
 *         zurueckgegeben, im Fehlerfall -1. Falls buf == NULL war, wird 0
 *         zurueckgegeben.
 */
int wctomb(char* buf, wchar_t wc);

/**
 * String aus breiten Zeichen in Multibyte-String umwandeln. Wird ein Zeichen
 * angetroffen, das nicht umgewandeld werden kann, wird -1 zurueckgegeben. Es
 * werden maximal len Bytes geschrieben. Das abschliessende L'\0' wird
 * mitkonvertiert, aber beim Rueckgabewert, wird es nicht mit beruecksichtigt.
 * Falls der String nicht vollstaendig konvertiert werden konnte, weil buf zu
 * klein ist, ist das Ergebnis nicht nullterminiert.
 *
 * Ist buf == NULL wird len ignoriert und es wird nur die Laenge bestimmt, die
 * der String haette, ohne das abschliessende 0-Byte.
 *
 * Um einen String vollstaendig zu konvertieren, muss buf mindestens die Groesse
 * wcstombs(NULL, wcs, 0) + 1 haben.
 *
 * @param buf Puffer in dem der Multibytestring abgelegt werden soll oder NULL
 * @param wcs Zeiger auf den String aus breiten Zeichen
 * @param len Groesse von buf
 *
 * @return Anzahl der in buf geschriebenen Bytes ohne abschliessendes '\0', oder
 *         (size_t) -1 wenn ein Zeichen nicht konvertiert werden konnte.
 */
size_t wcstombs(char* buf, const wchar_t* wcs, size_t len);

/**
 * Multibyte-String in String aus breiten Zeichen umwandeln. Wird ein Zeichen
 * angetroffen, das nicht umgewandeld werden kann, wird -1 zurueckgegeben. Es
 * werden maximal len breite Zeichen geschrieben. Das abschliessende '\0' wird
 * mitkonvertiert, aber beim Rueckgabewert, wird es nicht mit beruecksichtigt.
 * Falls der String nicht vollstaendig konvertiert werden konnte, weil buf zu
 * klein ist, ist das Ergebnis nicht L'\0'-terminiert.
 *
 * Ist buf == NULL wird len ignoriert und es wird nur die Laenge bestimmt, die
 * der String haette, ohne das abschliessende L'\0'.
 *
 * Um einen String vollstaendig zu konvertieren, muss buf mindestens die Groesse
 * von mbstowcs(NULL, str, 0) + 1 breiten Zeichen haben.
 *
 * @param buf Puffer in dem der String aus breiten Zeichen abgelegt werden soll
 *            oder NULL
 * @param wcs Zeiger auf den Multibytestring
 * @param len Groesse von buf in Zeichen
 *
 * @return Anzahl der in buf geschriebenen Zeichen ohne abschliessendes L'\0',
 *          oder (size_t) -1 wenn ein Zeichen nicht konvertiert werden konnte.
 */
size_t mbstowcs(wchar_t* buf, const char* str, size_t len);


/**
 * Seed fuer Zufallszahlgenerator setzen
 *
 * @param seed Seed
 */
void srand(unsigned int seed);

/**
 * Zufallszahl generieren
 *
 * @return Zufallszahl
 */
int rand(void);

/**
 * Zufallszahl generieren
 *
 * @return Zufallszahl
 */
long int random(void);

/**
 * Seed fuer Zufallszahlgenerator setzen
 *
 * @param seed Seed
 */
void srandom(unsigned int seed);


/// Dateinamen fuer temporaere Datei erstellen
char* mktemp(char* _template);

/// Temporaere Datei anlegen
int mkstemp(char* _template);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif