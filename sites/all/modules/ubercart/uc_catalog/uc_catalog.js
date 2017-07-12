(function ($) {
Drupal.behaviors.uc_catalog_gul76 = {
  attach: function (context, settings) {
    $("#btok").click (function(){
	var vfs = Drupal.settings.uc_catalog.myforms;
        $.each( Drupal.settings.uc_catalog.myforms, function(key, value) {
         if ($(value).find('input[name="qty"]').val() > 0) {
            $(value).submit();
         }
        });
    });
    var bfs = Drupal.settings.uc_catalog.mybuttons;
    $.each( bfs, function(key, value) {
		$(value).hide();
	});
  }
 }
})(jQuery);
