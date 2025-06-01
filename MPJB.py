class Solution:
    def jobScheduling(
        self, startTime: List[int], endTime: List[int], profit: List[int]) -> int:
        intervalo = sorted(zip(startTime, endTime, profit))  # ordena os trabalhos pelo horario de termino
        cache = {}  # armazena resultado

        def dfs(i):
            if i == len(intervalo):  # se chegou no final da lista retorna 0
                return 0
            if i in cache:  # se já calculou antes retorna cache
                return cache[i]

            # Não inclui o trabalho atual
            res = dfs(i + 1)

            # Inclui o trabalho atual
            j = bisect.bisect(intervalo, (intervalo[i][1], -1, -1))  # encontrar o próximo trabalho
            res = max(res, intervalo[i][2] + dfs(j))                 # lucro atual + resultado do próximo trabalho compatível
            cache[i] = res  # salva resultado no cache
            return res

        return dfs(0)  # começa busca a partir do 1 trabalho