#include <alsa/asoundlib.h>
#include <alsa/control.h>

char* getVol(void) {
    int vol;
    char* volBuf;
    snd_hctl_elem_t *elem;
    snd_hctl_t *hctl;
    snd_ctl_elem_id_t *id;
    snd_ctl_elem_value_t *control;
    snd_hctl_open(&hctl, "sysdefault:CARD=Generic_1", 1);
    snd_hctl_load(hctl);
    snd_ctl_elem_id_alloca(&id);
    snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_id_set_name(id, "Master Playback Volume");
    elem = snd_hctl_find_elem(hctl, id);
    snd_ctl_elem_value_alloca(&control);
    snd_ctl_elem_value_set_id(control, id);
    snd_hctl_elem_read(elem, control);
    vol = (int)snd_ctl_elem_value_get_integer(control,0);
    snd_hctl_close(hctl);

    volBuf = malloc(255 * sizeof(char));
    sprintf(volBuf, "%d", vol);
    return volBuf;
}

