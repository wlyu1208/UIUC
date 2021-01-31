from django.urls import path
from . import views
from django.conf.urls import url
from django.conf.urls import url, include
from django.contrib.auth import views as auth_views
from django.contrib.auth.views import LoginView, LogoutView

urlpatterns = [
    path('', views.home, name='home'),
    path('drinks', views.drinks, name='drinks'),
    path('drink_new', views.drink_new, name='drink_new'),
    path('drink_delete/<int:id>', views.drink_delete, name='drink_delete'),
    path('signup', views.signup, name='signup'),
    path('user/<slug:user_id>',views.user,name='user'),
    path('drinks/<int:id>',views.drink_view,name='drink_view'),
    path('login', views.login, name='login'),
    path('drinks/edit/<int:id>', views.drink_edit,name='drink_edit'),
    path('drinks/search', views.drink_search, name='drink_search'),
]