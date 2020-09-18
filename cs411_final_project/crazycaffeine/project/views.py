from django.shortcuts import render, redirect

from django.http import HttpResponse

from .forms import *
from django.urls import reverse_lazy, reverse
from .models import Drink
from .queries import get_drinks, add_user, calculate_recs

from django.db import connection


def drinks(request):
    data = dict()
    res = get_drinks()
    data['item'] = res
    return render(request, "project/drink_list.html", data)


def signup(request):
    if request.method == 'POST':
        form = UserCreateForm(request.POST)
        # drink name dropdown
        if form.is_valid():
            cd = form.cleaned_data
            # CHECK IF DRINK IS VALID
            add_user(cd['user_id'], cd['password'], cd['name'], cd['drink_name'],
                     cd['gender'], cd['height'], cd['weight'], cd['age'], cd['is_pregnant'])
            return user(request, cd['user_id'])
        else:  # found a match in User table
            print(form.errors)
            return render(request, "project/print_errors.html", {'form': form})
    else:
        data = dict()
        res = get_drinks()
        data['item'] = res
        return render(request, 'project/signup.html', data)


def login(request):
    if request.method == 'POST':
        form = LoginForm(request.POST)
        if form.is_valid():
            return HttpResponse('User does not exist!')
        else:  # found a match in User table
            Id = form.data.get('user_id')
            pword = form.data.get('password')
            with connection.cursor() as cursor:
                cursor.execute(
                    "SELECT * FROM user WHERE user_id = %s", [Id])
                query = cursor.fetchone()
                if (len(query) == 0):
                    return HttpResponse('User does not exist!')
                else:
                    if pword != query[1]:
                        return HttpResponse('Password does not match the user id')
                    else:
                        return user(request, Id)

    else:
        return render(request, 'project/login.html')


def user(request, user_id):
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM user WHERE user_id = %s", [user_id])
        query = cursor.fetchone()
    if (len(query) == 0):
        # no record exists
        return render(request, 'project/signup')
    else:
        reccs = calculate_recs(query[0])
        with connection.cursor() as cursor:
            if reccs[0] > 0 and reccs[2] > 0:
                if reccs[4] > reccs[0] and reccs[5] > reccs[2]:
                    cursor.execute(
                        "SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar > 0 AND g_sugar <= %s AND mg_caffeine > 0 AND mg_caffeine <= %s ORDER BY RAND() LIMIT 1", [reccs[0], reccs[2]])
                else:
                    if reccs[4] == 0 or reccs[5] == 0:
                        cursor.execute(
                            "SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar = 0 AND mg_caffeine = 0 ORDER BY RAND() LIMIT 1")
                    else:
                        cursor.execute(
                            "SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar > 0 AND g_sugar <= %s AND mg_caffeine > 0 AND mg_caffeine <= %s ORDER BY RAND() LIMIT 1", [reccs[5], reccs[4]])
            else:
                cursor.execute(
                    "SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar <= %s AND mg_caffeine <= %s ORDER BY RAND() LIMIT 1", [reccs[0], reccs[2]])
            below_both = cursor.fetchone()
        with connection.cursor() as cursor:
            cursor.execute(
                "SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar < 10 ORDER BY RAND() LIMIT 1")
            low_sugar = cursor.fetchone()
        with connection.cursor() as cursor:

            cursor.execute(
                "SELECT * FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE mg_caffeine <= 30 ORDER BY RAND() LIMIT 1")
            low_caff = cursor.fetchone()
        print(low_caff[4])
        data = {'user_id': query[0], 'name': query[2], 'drink_name': query[3], 'gender': query[4],
                'height': query[5], 'weight': query[6], 'age': query[7], 'pregnant': query[8], 'rec_sug': reccs[0], 'max_sug': reccs[1], 'rec_caf': reccs[2], 'max_caf': reccs[3], 'below_both': below_both[0], 'low_sugar': low_sugar[0], 'low_caff': low_caff[0], 'below_both_id': below_both[4], 'low_caff_id': low_caff[4], 'low_sug_id': low_sugar[4]}
        return render(request, 'project/user_info.html', data)


def drink_new(request):
    if request.method == 'POST':
        form = DrinkEditCreate(request.POST)
        if form.is_valid():
            with connection.cursor() as cursor:
                cursor.execute("INSERT INTO caffeine VALUES (%s, %s, %s, %s, %s)", [form.data.get("drink_name"), form.data.get(
                    "fl_oz"), form.data.get("mg_caffeine"), form.data.get("mg_floz"), form.data.get("id")])
                cursor.execute("INSERT INTO sugar VALUES (%s, %s, %s, %s, %s)", [form.data.get("drink_name"), form.data.get(
                    "fl_oz"), form.data.get("g_sugar"), form.data.get("g_floz"), form.data.get("id")])
            return drink_view(request, form.data.get("id"))
        else:  # found a match in User table
            print(form.errors)
            print(form.cleaned_data)
            return render(request, "project/print_errors.html", {'form': form})
    else:
        return render(request, 'project/drink_edit.html')


def drink_view(request, id):
    with connection.cursor() as cursor:
        cursor.execute(
            "SELECT * FROM caffeine JOIN sugar ON sugar.id = caffeine.id WHERE caffeine.id = %s", [id])
        query = cursor.fetchone()
    if query == None:
        return HttpResponse('This drink id doesnt exist')
    else:
        data = {'drink_name': query[0], 'fl_oz': query[1], 'mg_caffeine': query[2], 'mg_floz': query[3], 'Id': id,
                'g_sugar': query[7], 'g_floz': query[8]}

        return render(request, 'project/drink_view.html', data)


def drink_edit(request, id):
    if request.method == 'POST':
        with connection.cursor() as cursor:
            cursor.execute(
                "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar ON sugar.id = caffeine.id WHERE caffeine.id = %s", [id])
            query = cursor.fetchone()
        form = DrinkEditCreate(request.POST)
        if form.is_valid():
            with connection.cursor() as cursor:
                cursor.execute("UPDATE caffeine SET drink_name = %s,  fl_oz = %s, mg_caffeine = %s, mg_floz = %s, id = %s WHERE id = %s", [
                               form.data.get("drink_name"), form.data.get("fl_oz"), form.data.get("mg_caffeine"), form.data.get("mg_floz"), form.data.get("id"), id])
                cursor.execute("UPDATE sugar SET drink_name = %s,  fl_oz = %s, g_sugar = %s, g_floz = %s, id = %s WHERE id = %s", [
                               form.data.get("drink_name"), form.data.get("fl_oz"), form.data.get("g_sugar"), form.data.get("g_floz"), form.data.get("id"), id])
            return drink_view(request, form.data.get("id"))
        else:  # found a match in User table
            print(form.errors)
            print(form.cleaned_data)
            return render(request, "project/print_errors.html", {'form': form})
    else:
        return render(request, 'project/drink_edit.html')


def drink_delete(request, id):
    if request.method == 'POST':
        with connection.cursor() as cursor:
            cursor.execute("DELETE FROM sugar WHERE id = %s", [id])
            cursor.execute("DELETE FROM caffeine WHERE id = %s", [id])
        return drinks(request)
    else:
        with connection.cursor() as cursor:
            cursor.execute(
                "SELECT caffeine.drink_name FROM caffeine WHERE caffeine.id = %s", [id])
            query = cursor.fetchone()
        data = dict()
        data['drink'] = query
        return render(request, 'project/drink_delete.html', data)


def drink_search(request):
    if request.method == 'POST':
        form = DrinkFilter(request.POST)
        name_filt = True
        caff_filt = True
        sug_filt = True
        if form.is_valid():
            cd = form.cleaned_data
            if cd['g_sug'] == 0:
                sug_filt = False
            if cd['mg_caff'] == 0:
                caff_filt = False
            if cd['drink_name'] == 'None':
                name_filt = False
            if name_filt and caff_filt and sug_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE caffeine.drink_name LIKE {0}{2}{1}{2}{0} AND g_sugar < {3} AND mg_caff < {4}".format(
                        '\"', cd['drink_name'], '%', cd['g_sug'], cd['mg_caff'])
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                return render(request, 'project/drink_list.html', data)
            if name_filt and caff_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE caffeine.drink_name LIKE {0}{2}{1}{2}{0} AND mg_caffeine < {3}".format(
                        '\"', cd['drink_name'], '%', cd['mg_caff'])
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                return render(request, 'project/drink_list.html', data)

            if name_filt and sug_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE caffeine.drink_name LIKE {0}{2}{1}{2}{0} AND g_sugar < {3}".format(
                        '\"', cd['drink_name'], '%', cd['g_sug'])
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                return render(request, 'project/drink_list.html', data)
            if name_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE caffeine.drink_name LIKE {0}{2}{1}{2}{0}".format(
                        '\"', cd['drink_name'], '%')
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                print(data)
                return render(request, 'project/drink_list.html', data)
            if caff_filt and sug_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar < {0} AND mg_caffeine < {1}".format(
                        cd['g_sug'], cd['mg_caff'])
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                return render(request, 'project/drink_list.html', data)
            if caff_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE mg_caffeine < {0}".format(
                        cd['mg_caff'])
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                return render(request, 'project/drink_list.html', data)
            if sug_filt:
                with connection.cursor() as cursor:
                    to_run = "SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz, caffeine.id FROM caffeine JOIN sugar on sugar.id = caffeine.id WHERE g_sugar < {0}".format(
                        cd['g_sug'])
                    cursor.execute(to_run)
                    query = cursor.fetchall()
                data = dict()
                data['item'] = query
                return render(request, 'project/drink_list.html', data)
            return HttpResponse('That is not a valid filter')
        else:
            print(form.errors)
            print(form.cleaned_data)
            return render(request, "project/print_errors.html", {'form': form})
        return render(request, 'project/drink_list.html')
    else:
        return render(request, 'project/drink_filter.html')


def home(request):
    return render(request, 'project/home.html')