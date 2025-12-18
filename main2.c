#include <systemd/sd-bus.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    sd_bus *bus = NULL;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    int synced = 0;
    int r;

    /* Connexion au bus système */
    r = sd_bus_default_system(&bus);
    if (r < 0) {
        fprintf(stderr, "Erreur bus system: %s\n", strerror(-r));
        return -1;
    }

    /* Lecture de la propriété NTPSynchronized */
    r = sd_bus_get_property_trivial(
        bus,
        "org.freedesktop.timedate1",        /* service */
        "/org/freedesktop/timedate1",       /* objet */
        "org.freedesktop.timedate1",        /* interface */
        "NTPSynchronized",                  /* propriété */
        &error,
        'b',                                /* signature D-Bus : boolean */
        &synced                             /* stocké dans int */
    );

    if (r < 0) {
        fprintf(stderr, "Erreur lecture NTPSynchronized: %s\n",
                error.message ? error.message : strerror(-r));
        sd_bus_error_free(&error);
        sd_bus_unref(bus);
        return -1;
    }

    sd_bus_unref(bus);

    printf("valeur : %d\n", synced);

    if (synced == 1) {
        printf("NTP synchronisé\n");
    } else if (synced == 0) {
        printf("NTP NON synchronisé\n");
    } else {
        printf("Erreur D-Bus\n");
    }

    /* sd_bus *bus = NULL;
    sd_bus_default_system(&bus);

    int synced = 0;
    int r = sd_bus_get_property_trivial(
    bus,
    "org.freedesktop.timedate1",      // bus name
    "/org/freedesktop/timedate1",     // object path
    "org.freedesktop.timedate1",      // interface
    "NTPSynchronized",                 // property
    NULL,                              // error (sd_bus_error*) si tu veux
    'b',                               // signature D-Bus (bool)
    &synced
);


    if (r < 0) {
        fprintf(stderr, "Erreur lecture NTPSynchronized: %s\n", strerror(-r));
        return 1;
    }

    if (synced)
        printf("NTP synchronisé\n");
    else
        printf("NTP perdu ou non synchronisé\n");


    sd_bus_unref(bus);
    return 0; */
}
