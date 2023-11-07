void knightMove(String movements){
    movements.toLowerCase();
    //Horse movement for a 16cm square chess
    //First move Left second move up
    if(movements=="is"){
        turn('L',90);
        goStraight('F',32);
        turn('R',90);
        goStraight('F',16);
    //First move Left second move down
    }else if(movements=="ib"){
        turn('L',90);
        goStraight('F',32);
        turn('R',90);
        goStraight('B',16);
    //First move right second move up
    }else if(movements=="ds"){
        turn('R',90);
        goStraight('F',32);
        turn('L',90);
        goStraight('F',16);
    //First move right second move down
    }else if(movements=="db"){
        turn('R',90);
        goStraight('F',32);
        turn('L',90);
        goStraight('B',16);
    //First move down second move left
    }else if(movements=="bi"){
        goStraight('B',32);
        turn('L',90);
        goStraight('F',16);
        turn('R',90);
    //First move down second move right
    }else if(movements=="bd"){
        goStraight('B',32);
        turn('R',90);
        goStraight('F',16);
        turn('L',90);
    //First move up second move left
    }else if(movements=="si"){
        goStraight('F',32);
        turn('L',90);
        goStraight('F',16);
        turn('R',90);
    //First move up second move right
    }else if(movements=="sd"){
        goStraight('F',32);
        turn('R',90);
        goStraight('F',16);
        turn('L',90);
    }else{
         // Nothing is done in other cases
        // You can include that it shows an error log
        Serial.println("El movimiento: "+movements+" es un movimiento inválido. Aprende a leer instrucciones.");
        turn('R',90);
        turn('L',180);
        turn('R',90);
    }
}

void queenMove(String movements){
    movements.toLowerCase();
    int magnitude=movements[2]-'0';
    float diagonalLength = 22.627416997969520780827019587355;
    float diagonalMovementLength = magnitude*diagonalLength;
    float straightMovementLength = magnitude*16;
    if(movements[0]=='s'){
        if(movements[1]=='d'){
            turn('R',45);
            goStraight('F', diagonalMovementLength);
            turn('L',45);
        }else if(movements[1]=='i'){
            turn('L',45);
            goStraight('F', diagonalMovementLength);
            turn('R',45);
        }else{
            goStraight('F', straightMovementLength);
        }
    }else if(movements[0]=='b'){
        if(movements[1]=='d'){
            turn('R',90+45);
            goStraight('F', diagonalMovementLength);
            turn('L',90+45);
        }else if(movements[1]=='i'){
            turn('L',90+45);
            goStraight('F', diagonalMovementLength);
            turn('R',90+45);
        }else{
            goStraight('B', straightMovementLength);
        }

    }else if(movements[0]=='d'){
        turn('R',90);
        goStraight('F', straightMovementLength);
        turn('L',90);
    }else if(movements[0]=='i'){
        turn('L',90);
        goStraight('F', straightMovementLength);
        turn('R',90);
    }

}

void bishopMove(String movements){
    movements.toLowerCase();
    int magnitude=movements[2]-'0';
    float diagonalLength = 22.627416997969520780827019587355;
    float diagonalMovementLength = magnitude*diagonalLength;
    if(movements[0]=='s'){
        if(movements[1]=='D'||movements[1]=='d'){
            turn('R',45);
            goStraight('F', diagonalMovementLength);
            turn('L',45);
        }else if(movements[1]=='i'){
            turn('L',45);
            goStraight('F', diagonalMovementLength);
            turn('R',45);
        }
    }else if(movements[0]=='b'){
        if(movements[1]=='d'){
            turn('R',90+45);
            goStraight('F', diagonalMovementLength);
            turn('L',90+45);
        }else if(movements[1]=='i'){
            turn('L',90+45);
            goStraight('F', diagonalMovementLength);
            turn('R',90+45);
        }
    }

}

void rookMove(String movements){
    movements.toLowerCase();
    int magnitude=movements[1]-'0';
    float straightMovementLength = magnitude*16;
    if(movements[0]=='s'){
        goStraight('F', straightMovementLength);
    }else if(movements[0]=='b'){
        goStraight('B', straightMovementLength);
    }else if(movements[0]=='d'){
        turn('R',90);
        goStraight('F', straightMovementLength);
        turn('L',90);
    }else if(movements[0]=='i'){
        turn('L',90);
        goStraight('F', straightMovementLength);
        turn('R',90);
    }

}

void kingMove(String movements){
    movements.toLowerCase();
    int magnitude=1;
    float diagonalLength = 22.627416997969520780827019587355;
    float diagonalMovementLength = magnitude*diagonalLength;
    float straightMovementLength = magnitude*16;
    if(movements[0]=='s'){
        if(movements[1]=='d'){
            turn('R',45);
            goStraight('F', diagonalMovementLength);
            turn('L',45);
        }else if(movements[1]=='i'){
            turn('L',45);
            goStraight('F', diagonalMovementLength);
            turn('R',45);
        }else{
            goStraight('F', straightMovementLength);
        }
    }else if(movements[0]=='b'){
        if(movements[1]=='d'){
            turn('R',90+45);
            goStraight('F', diagonalMovementLength);
            turn('L',90+45);
        }else if(movements[1]=='i'){
            turn('L',90+45);
            goStraight('F', diagonalMovementLength);
            turn('R',90+45);
        }else{
            goStraight('B', straightMovementLength);
        }

    }else if(movements[0]=='d'){
        turn('R',90);
        goStraight('F', straightMovementLength);
        turn('L',90);
    }else if(movements[0]=='i'){
        turn('L',90);
        goStraight('F', straightMovementLength);
        turn('R',90);
    }

}


void pawnMove(String movements){
    movements.toLowerCase();
    float diagonalLength = 22.627416997969520780827019587355;
    if(movements[0]=='a'){
        if(movements[1]=='1'){
            goStraight('F',16);
        }else if(movements[1]=='2'){
            goStraight('F',32);
        }
    }else if(movements[0]=='c'){
        if(movements[1]=='i'){
            turn('L',45);
            goStraight('F',diagonalLength);
            turn('R',45);
        }else if(movements[1]=='d'){
            turn('R',45);
            goStraight('F',diagonalLength);
            turn('L',45);
        }
    }
}
