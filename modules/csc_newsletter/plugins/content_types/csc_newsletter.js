(function($) {

    $(document).ready(function() {
        $('.mce-button').on("click", function(e) {
            e.preventDefault();

            var f = $(this).closest("form"), email = f.find('input[name=EMAIL]');

            //Check email field
            if (email.val() == '') {
                alert('Email field is required');
                email.focus();
                return;
            }

            $("#dialog-modal").dialog({
                height : 355,
                width : 400,
                modal : true,
                buttons : {
                    "Cancel" : function() {
                        $(this).dialog("close");
                    },
                    "Subscribe" : function() {
                        if ($('#news-1').prop('checked'))
                            $('#mce-group-16045-0').prop('checked', true);
                        else
                            $('#mce-group-16045-0').prop('checked', false);

                        if ($('#modal-form #news-2').prop('checked'))
                            $('#mce-group-16045-1').prop('checked', true);
                        else
                            $('#mce-group-16045-1').prop('checked', false);

                        if ($('#modal-form #news-3').prop('checked'))
                            $('#mce-group-16045-2').prop('checked', true);
                        else
                            $('#mce-group-16045-2').prop('checked', false);

                        if ($('#modal-form #news-4').prop('checked'))
                            $('#mce-group-16045-4').prop('checked', true);
                        else
                            $('#mce-group-16045-4').prop('checked', false);

                        f.submit();
                        $(this).dialog("close");
                    }
                }
            });
        });
    });

})(jQuery);
