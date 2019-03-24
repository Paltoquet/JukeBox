import QtQuick 2.0
import QtMultimedia 5.8

Item {
    id: root

    property var pistes: [ piste1, piste2, piste3, piste4, piste5, piste6, piste7 ]

    function getAvailablePiste(){
        for(var i = 0; i < root.pistes.length; i++) {
            var pist = root.pistes[i];
            console.log(pist);
            if(pist.playbackState !== MediaPlayer.PlayingState) {
                return pist;
            }
        }
        return null;
    }

    function run(path){
        var availablePiste = root.getAvailablePiste();
        if(availablePiste) {
            availablePiste.source = path;
            availablePiste.play();
        }
    }

    Audio {
        id: piste1
    }

    Audio {
        id: piste2
    }

    Audio {
        id: piste3
    }

    Audio {
        id: piste4
    }

    Audio {
        id: piste5
    }

    Audio {
        id: piste6
    }

    Audio {
        id: piste7
    }
}
