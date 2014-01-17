<?php

    $data = json_decode(file_get_contents('php://input'));

    if ($data['message'] == 'hello' ) {

        $reply = array('reply' => 'hi');
        return json_encode($reply);
    }

?>
